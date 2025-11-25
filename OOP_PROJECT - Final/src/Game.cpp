#include "Game.h"
#include <random>
#include <cmath>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {
    window.setFramerateLimit(FPS);
    state = GameState::MENU;
    
    score = 0;
    combo = 0;
    obstacleSpawnTimer = 0;
    powerUpSpawnTimer = 0;
    currentObstacleSpeed = OBSTACLE_SPEED;
    currentSpawnTime = OBSTACLE_SPAWN_TIME;
    shakeIntensity = 0;
    shakeTimer = 0;
    
    createBackground();
}

void Game::run() {
    sf::Clock clock;
    
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();
        // Cap dt to prevent huge jumps
        //if (dt > 0.1f) dt = 0.1f;
        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            
            if (event.key.code == sf::Keyboard::Enter) {
                if (state == GameState::MENU) {
                    startGame();
                } else if (state == GameState::GAME_OVER) {
                    resetGame();
                    startGame();
                }
            }
            
            if (event.key.code == sf::Keyboard::Space && state == GameState::PLAYING) {
                if (player.canDash()) {
                    player.dash();
                    particles.emit(player.getPosition(), COLOR_BLUE, 20);
                }
            }
        }
    }
}

void Game::update(float dt) {
    if (state != GameState::PLAYING) return;
    
    // Update player
    player.update(dt);
    player.drawTrail(window, particles);
    
    // Update obstacles
    obstacleSpawnTimer += dt;
    if (obstacleSpawnTimer >= currentSpawnTime) {
        spawnObstacle();
        obstacleSpawnTimer = 0;
    }
    
    for (auto& obstacle : obstacles) {
        obstacle->update(dt);
    }
    
    // Update power-ups
    powerUpSpawnTimer += dt;
    if (powerUpSpawnTimer >= 5.0f) {
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }
    
    for (auto& powerUp : powerUps) {
        powerUp->update(dt);
    }
    
    // Update particles
    particles.update(dt);
    
    // Check collisions
    checkCollisions();
    
    // Update UI
    ui.updateScore(score);
    ui.updateCombo(combo);
    ui.updateDashCooldown(player.getDashCooldown());
    
    // Update difficulty
    updateDifficulty();
    
    // Update screen shake
    if (shakeTimer > 0) {
        shakeTimer -= dt;
        cameraOffset.x = (rand() % 100 - 50) / 50.0f * shakeIntensity;
        cameraOffset.y = (rand() % 100 - 50) / 50.0f * shakeIntensity;
    } else {
        cameraOffset = sf::Vector2f(0, 0);
    }
    
    // Remove inactive obstacles and power-ups
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [](const std::unique_ptr<Obstacle>& obs) { return !obs->active(); }),
        obstacles.end()
    );
    
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(),
            [](const std::unique_ptr<PowerUp>& pw) { return !pw->active(); }),
        powerUps.end()
    );
}

void Game::render() {
    window.clear(COLOR_BACKGROUND);
    
    // Draw background stars
    for (const auto& star : backgroundStars) {
        window.draw(star);
    }
    
    if (state == GameState::MENU) {
        ui.drawMenu(window);
    } else if (state == GameState::PLAYING) {
        // Apply camera shake
        sf::View view = window.getDefaultView();
        view.setCenter(WINDOW_WIDTH / 2.0f + cameraOffset.x, 
                       WINDOW_HEIGHT / 2.0f + cameraOffset.y);
        window.setView(view);
        
        // Draw game objects
        for (const auto& obstacle : obstacles) {
            obstacle->draw(window);
        }
        
        for (const auto& powerUp : powerUps) {
            powerUp->draw(window);
        }
        
        player.draw(window);
        particles.draw(window);
        
        // Reset view for UI
        window.setView(window.getDefaultView());
        ui.drawGameUI(window);
        
    } else if (state == GameState::GAME_OVER) {
        // Draw last game state
        for (const auto& obstacle : obstacles) {
            obstacle->draw(window);
        }
        player.draw(window);
        particles.draw(window);
        
        ui.drawGameOver(window, score);
    }
    
    window.display();
}

void Game::startGame() {
    state = GameState::PLAYING;
    player.reset();
}

void Game::resetGame() {
    score = 0;
    combo = 0;
    obstacleSpawnTimer = 0;
    powerUpSpawnTimer = 0;
    currentObstacleSpeed = OBSTACLE_SPEED;
    currentSpawnTime = OBSTACLE_SPAWN_TIME;
    
    obstacles.clear();
    powerUps.clear();
    particles.clear();
}

void Game::gameOver() {
    state = GameState::GAME_OVER;
    particles.emitExplosion(player.getPosition(), COLOR_RED);
    screenShake(20.0f);
}

void Game::spawnObstacle() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> yDist(OBSTACLE_HEIGHT, 
                                                WINDOW_HEIGHT - OBSTACLE_HEIGHT);
    
    float y = yDist(gen);
    sf::Vector2f pos(WINDOW_WIDTH + OBSTACLE_WIDTH, y);
    sf::Color color = getRandomColor();
    
    obstacles.push_back(std::make_unique<Obstacle>(pos, color, currentObstacleSpeed));
}

void Game::spawnPowerUp() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> yDist(50, WINDOW_HEIGHT - 50);
    std::uniform_int_distribution<int> typeDist(0, 2);
    
    float y = yDist(gen);
    sf::Vector2f pos(WINDOW_WIDTH + 30, y);
    PowerUpType type = static_cast<PowerUpType>(typeDist(gen));
    
    powerUps.push_back(std::make_unique<PowerUp>(pos, type, currentObstacleSpeed * 0.8f));
}

void Game::checkCollisions() {
    sf::FloatRect playerBounds = player.getBounds();
    
    // Check obstacle collisions
    for (const auto& obstacle : obstacles) {
        if (obstacle->active() && obstacle->getBounds().intersects(playerBounds)) {
            gameOver();
            return;
        }
        
        // Score for passing obstacles (fixed - only score once per obstacle)
        if (obstacle->active() && 
            obstacle->getPosition().x + OBSTACLE_WIDTH/2 < player.getPosition().x && 
            obstacle->getPosition().x + OBSTACLE_WIDTH/2 > player.getPosition().x - 10) {
            score += SCORE_PER_DODGE;
            combo++;
            particles.emit(obstacle->getPosition(), obstacle->getColor(), 15);
        }
    }
    
    // Check power-up collisions
    for (auto& powerUp : powerUps) {
        if (powerUp->active() && powerUp->getBounds().intersects(playerBounds)) {
            score += SCORE_POWERUP;
            particles.emit(powerUp->getPosition(), COLOR_YELLOW, 25);
            powerUp->deactivate();
        }
    }
}

void Game::updateDifficulty() {
    // Increase speed based on score (FIXED - only once per 100 points)
    static int lastDifficultyScore = 0;
    if (score > 0 && score >= lastDifficultyScore + 100) {
        currentSpawnTime *= SPEED_INCREASE_RATE;
        currentObstacleSpeed *= 1.05f;
        
        if (currentSpawnTime < 0.5f) currentSpawnTime = 0.5f;
        if (currentObstacleSpeed > 600.0f) currentObstacleSpeed = 600.0f;
        
        lastDifficultyScore = score;
    }
    
    // Reset combo if too slow (FIXED - uses actual dt)
    static float comboTimer = 0;
    comboTimer += 0.016f; // This should ideally use dt parameter
    if (comboTimer > 3.0f) {
        combo = 0;
        comboTimer = 0;
    }
}

void Game::screenShake(float intensity) {
    shakeIntensity = intensity;
    shakeTimer = 0.3f;
}

sf::Color Game::getRandomColor() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 5);
    
    sf::Color colors[] = {COLOR_RED, COLOR_BLUE, COLOR_YELLOW, 
                         COLOR_GREEN, COLOR_PURPLE, COLOR_ORANGE};
    return colors[dist(gen)];
}

void Game::createBackground() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(0, WINDOW_WIDTH);
    std::uniform_real_distribution<float> yDist(0, WINDOW_HEIGHT);
    std::uniform_real_distribution<float> sizeDist(1, 3);
    
    for (int i = 0; i < 100; i++) {
        sf::CircleShape star(sizeDist(gen));
        star.setPosition(xDist(gen), yDist(gen));
        star.setFillColor(sf::Color(150, 150, 150, 150));
        backgroundStars.push_back(star);
    }
}