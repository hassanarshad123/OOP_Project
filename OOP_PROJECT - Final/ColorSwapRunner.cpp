#include "ColorSwapRunner.hpp"
#include <iostream>

// ==================== PLAYER IMPLEMENTATION ====================

Player::Player(float x, float y, float size)
{
    // Basic setup of player values
    velocity = sf::Vector2f(0.f, 0.f);
    gravity = 800.f;
    jumpStrength = -410.f;
    moveSpeed = 300.f;
    isOnGround = false;
    groundLevel = 500.f;
    currentColor = COLOR_RED;
    canSwapColor = true;

    // Shape (visual)
    shape.setSize(sf::Vector2f(size, size));
    shape.setPosition(x, y);
    updateColor();
}

void Player::updateColor() {
    // Change SFML color based on our PlayerColor enum
    switch (currentColor) {
        case COLOR_RED:    shape.setFillColor(sf::Color::Red);    break;
        case COLOR_GREEN:  shape.setFillColor(sf::Color::Green);  break;
        case COLOR_BLUE:   shape.setFillColor(sf::Color::Blue);   break;
        case COLOR_YELLOW: shape.setFillColor(sf::Color::Yellow); break;
    }
}

void Player::swapColor() {
    // Only allow swap if cooldown is ready
    if (canSwapColor) {
        // Cycle between 4 colors
        switch (currentColor) {
            case COLOR_RED:    currentColor = COLOR_GREEN;  break;
            case COLOR_GREEN:  currentColor = COLOR_BLUE;   break;
            case COLOR_BLUE:   currentColor = COLOR_YELLOW; break;
            case COLOR_YELLOW: currentColor = COLOR_RED;    break;
        }

        updateColor();              // update the SFML color
        canSwapColor = false;       // lock swapping for a bit
        swapCooldownClock.restart();
    }
}

void Player::handleInput() {
    // Check if we can enable color swap again
    if (!canSwapColor && swapCooldownClock.getElapsedTime().asSeconds() > 0.2f) {
        canSwapColor = true;
    }

    // Color swap with C key
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
        swapColor();
    }

    // Horizontal movement
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
        sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        velocity.x = -moveSpeed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
             sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        velocity.x = moveSpeed;
    }
    else {
        velocity.x = 0.f;
    }

    // Jump only when on ground
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && isOnGround) {
        velocity.y = jumpStrength;  // jump upwards
        isOnGround = false;
    }
}

void Player::update(float dt) {
    // First, handle input (keys)
    handleInput();

    // Apply gravity every frame
    velocity.y += gravity * dt;

    // Move the shape according to velocity
    shape.move(velocity.x * dt, velocity.y * dt);

    // Simple floor collision
    float bottom = shape.getPosition().y + shape.getSize().y;
    if (bottom >= groundLevel) {
        shape.setPosition(shape.getPosition().x, groundLevel - shape.getSize().y);
        velocity.y = 0.f;
        isOnGround = true;
    }
    else {
        isOnGround = false;
    }

    // Keep player inside window horizontally
    if (shape.getPosition().x < 0.f) {
        shape.setPosition(0.f, shape.getPosition().y);
    }
    if (shape.getPosition().x + shape.getSize().x > 800.f) {
        shape.setPosition(800.f - shape.getSize().x, shape.getPosition().y);
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getBounds() const {
    return shape.getGlobalBounds();
}

void Player::reset(float x, float y) {
    shape.setPosition(x, y);
    velocity = sf::Vector2f(0.f, 0.f);
    isOnGround = false;
    currentColor = COLOR_RED;
    updateColor();
    canSwapColor = true;
}

PlayerColor Player::getColor() const {
    return currentColor;
}

float Player::getX() const {
    return shape.getPosition().x;
}

// ==================== OBSTACLE IMPLEMENTATION ====================

Obstacle::Obstacle(float x, float y, float width, float height, float speedValue, PlayerColor c)
{
    speed = speedValue;
    resetX = x;
    color = c;
    hasCounted = false;

    shape.setSize(sf::Vector2f(width, height));
    shape.setPosition(x, y);
    updateColor();
}

void Obstacle::updateColor() {
    switch (color) {
        case COLOR_RED:    shape.setFillColor(sf::Color::Red);    break;
        case COLOR_GREEN:  shape.setFillColor(sf::Color::Green);  break;
        case COLOR_BLUE:   shape.setFillColor(sf::Color::Blue);   break;
        case COLOR_YELLOW: shape.setFillColor(sf::Color::Yellow); break;
    }
}

void Obstacle::update(float dt) {
    // Move left
    shape.move(-speed * dt, 0.f);

    // If offscreen, send it back to right side
    if (isOffScreen()) {
        reset();
    }
}

void Obstacle::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Obstacle::getBounds() const {
    return shape.getGlobalBounds();
}

void Obstacle::reset() {
    // Move back to resetX (same height)
    shape.setPosition(resetX, shape.getPosition().y);
    hasCounted = false;
}

void Obstacle::setColor(PlayerColor newColor) {
    color = newColor;
    updateColor();
}

bool Obstacle::isOffScreen() const {
    return shape.getPosition().x + shape.getSize().x < 0.f;
}

PlayerColor Obstacle::getColor() const {
    return color;
}

bool Obstacle::shouldCount(float playerX) {
    // Score increases when obstacle fully passes the player (only once)
    float rightSide = shape.getPosition().x + shape.getSize().x;
    if (!hasCounted && rightSide < playerX) {
        hasCounted = true;
        return true;
    }
    return false;
}

float Obstacle::getX() const {
    return shape.getPosition().x;
}

// ==================== LEVEL MANAGER IMPLEMENTATION ====================

LevelManager::LevelManager() {
    spawnTimer = 0.f;
    spawnInterval = 2.5f;
    obstaclesPassed = 0;
    colorIndex = 0;
    heightIndex = 0;

    // First obstacle
    float height = getRandomHeight();
    float yPos = 500.f - height;
    obstacles.push_back(new Obstacle(800.f, yPos, 40.f, height, 200.f, getRandomColor()));
}

LevelManager::~LevelManager() {
    // Delete all obstacle pointers
    for (int i = 0; i < (int)obstacles.size(); i++) {
        delete obstacles[i];
    }
}

PlayerColor LevelManager::getRandomColor() {
    // Fixed simple pattern instead of real random
    PlayerColor colors[4] = {
        COLOR_RED,
        COLOR_GREEN,
        COLOR_BLUE,
        COLOR_YELLOW
    };

    PlayerColor result = colors[colorIndex];
    colorIndex = (colorIndex + 1) % 4;
    return result;
}

float LevelManager::getRandomHeight() {
    // Different heights for variety
    float heights[3] = { 50.f, 100.f, 200.f };

    float result = heights[heightIndex];
    heightIndex = (heightIndex + 1) % 3;
    return result;
}

void LevelManager::update(float dt, float playerX) {
    // Update all obstacles and count score
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->update(dt);

        if (obstacles[i]->shouldCount(playerX)) {
            obstaclesPassed++;
        }
    }

    // Spawn new obstacles after some time
    spawnTimer += dt;
    if (spawnTimer >= spawnInterval) {
        float height = getRandomHeight();
        float yPos = 500.f - height;
        obstacles.push_back(new Obstacle(800.f, yPos, 40.f, height, 200.f, getRandomColor()));
        spawnTimer = 0.f;
    }

    // Remove obstacles that are far offscreen
    int i = 0;
    while (i < (int)obstacles.size()) {
        if (obstacles[i]->getX() < -200.f) {
            delete obstacles[i];
            obstacles.erase(obstacles.begin() + i);
        } else {
            i++;
        }
    }
}

void LevelManager::draw(sf::RenderWindow& window) {
    for (int i = 0; i < (int)obstacles.size(); i++) {
        obstacles[i]->draw(window);
    }
}

void LevelManager::checkCollisions(const sf::FloatRect& playerBounds,
                                   PlayerColor playerColor,
                                   bool& collision) {
    // Check every obstacle for collision with player
    for (int i = 0; i < (int)obstacles.size(); i++) {
        if (playerBounds.intersects(obstacles[i]->getBounds())) {
            // Only lose if colors are different
            if (playerColor != obstacles[i]->getColor()) {
                collision = true;
                return;
            }
        }
    }
}

void LevelManager::reset() {
    // Delete old obstacles
    for (int i = 0; i < (int)obstacles.size(); i++) {
        delete obstacles[i];
    }
    obstacles.clear();

    spawnTimer = 0.f;
    obstaclesPassed = 0;
    colorIndex = 0;
    heightIndex = 0;

    // Create first obstacle again
    float height = getRandomHeight();
    float yPos = 500.f - height;
    obstacles.push_back(new Obstacle(800.f, yPos, 40.f, height, 200.f, getRandomColor()));
}

int LevelManager::getScore() const {
    return obstaclesPassed;
}

// ==================== WELCOME SCREEN IMPLEMENTATION ====================

WelcomeScreen::WelcomeScreen(const sf::Font& font, Game* gamePtr)
{
    game = gamePtr;

    // Title
    titleText.setFont(font);
    titleText.setString("Color Swap Runner");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(150.f, 150.f);

    // Start hint
    hintText.setFont(font);
    hintText.setString("Press Enter to Start");
    hintText.setCharacterSize(30);
    hintText.setFillColor(sf::Color::Green);
    hintText.setPosition(250.f, 300.f);

    // Controls
    controlsText.setFont(font);
    controlsText.setString(
        "Controls:\n"
        "A/D or Arrow Keys - Move\n"
        "Space - Jump\n"
        "C - Change Color"
    );
    controlsText.setCharacterSize(20);
    controlsText.setFillColor(sf::Color::Cyan);
    controlsText.setPosition(230.f, 380.f);
}

void WelcomeScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Enter) {
        // Go to game screen
        game->switchScreen(SCREEN_GAME);
    }
}

void WelcomeScreen::update(float) {
    // Could add simple animations later, currently nothing
}

void WelcomeScreen::draw(sf::RenderWindow& window) {
    window.draw(titleText);
    window.draw(hintText);
    window.draw(controlsText);
}

// ==================== GAME SCREEN IMPLEMENTATION ====================

GameScreen::GameScreen(const sf::Font& font, Game* gamePtr)
    : player(100.f, 400.f, 40.f)
{
    game = gamePtr;
    isGameOver = false;

    // Score text
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    // Instruction
    instructionText.setFont(font);
    instructionText.setString("Press C to change color!");
    instructionText.setCharacterSize(18);
    instructionText.setFillColor(sf::Color::Cyan);
    instructionText.setPosition(10.f, 40.f);

    // Game over message
    gameOverText.setFont(font);
    gameOverText.setString("Game Over - Press ESC");
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200.f, 250.f);
}

void GameScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape) {
        // Reset and go back to welcome screen
        reset();
        game->switchScreen(SCREEN_WELCOME);
    }
}

void GameScreen::update(float dt) {
    if (!isGameOver) {
        player.update(dt);
        levelManager.update(dt, player.getX());

        int score = levelManager.getScore();
        scoreText.setString("Score: " + std::to_string(score));

        checkCollision();
    }
}

void GameScreen::draw(sf::RenderWindow& window) {
    player.draw(window);
    levelManager.draw(window);
    window.draw(scoreText);
    window.draw(instructionText);

    if (isGameOver) {
        window.draw(gameOverText);
    }
}

void GameScreen::reset() {
    player.reset(100.f, 400.f);
    levelManager.reset();
    isGameOver = false;
}

void GameScreen::checkCollision() {
    bool collision = false;
    levelManager.checkCollisions(player.getBounds(), player.getColor(), collision);
    if (collision) {
        isGameOver = true;
    }
}

// ==================== GAME IMPLEMENTATION ====================

Game::Game()
    : window(sf::VideoMode(800, 600), "Color Swap Runner")
{
    currentScreen = nullptr;

    window.setFramerateLimit(60);
    loadFont();
    createScreen(SCREEN_WELCOME);
}

Game::~Game() {
    delete currentScreen;
}

void Game::loadFont() {
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font! Trying system font..." << std::endl;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Could not load any font!" << std::endl;
        }
    }
}

void Game::createScreen(ScreenType type) {
    // Delete old screen
    if (currentScreen != nullptr) {
        delete currentScreen;
        currentScreen = nullptr;
    }

    // Create new screen
    if (type == SCREEN_WELCOME) {
        currentScreen = new WelcomeScreen(font, this);
    }
    else if (type == SCREEN_GAME) {
        currentScreen = new GameScreen(font, this);
    }
}

void Game::switchScreen(ScreenType type) {
    createScreen(type);
}

void Game::run() {
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (currentScreen != nullptr) {
                currentScreen->handleEvent(event);
            }
        }

        if (currentScreen != nullptr) {
            currentScreen->update(dt);
        }

        window.clear(sf::Color::Black);
        if (currentScreen != nullptr) {
            currentScreen->draw(window);
        }
        window.display();
    }
}
