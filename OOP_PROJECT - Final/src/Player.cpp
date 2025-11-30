#include "Player.h"
#include <cmath>

Player::Player() {
    position = sf::Vector2f(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f);
    shape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));
    shape.setOrigin(PLAYER_SIZE / 2, PLAYER_SIZE / 2);

    // Initialize available colors for color swapping
    availableColors.push_back(COLOR_RED);
    availableColors.push_back(COLOR_BLUE);
    availableColors.push_back(COLOR_YELLOW);
    availableColors.push_back(COLOR_GREEN);
    availableColors.push_back(COLOR_PURPLE);
    availableColors.push_back(COLOR_ORANGE);

    // Start with first color (RED)
    currentColorIndex = 0;
    currentColor = availableColors[currentColorIndex];
    shape.setFillColor(currentColor);
    shape.setOutlineThickness(3.0f);
    shape.setOutlineColor(sf::Color::White);

    isDashing = false;
    dashTimer = 0;
    dashCooldownTimer = 0;
    trailTimer = 0;
}

void Player::handleInput() {
    velocity = sf::Vector2f(0, 0);
    
    if (!isDashing) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -PLAYER_SPEED;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = PLAYER_SPEED;
        }
        
        // Normalize diagonal movement
        if (velocity.x != 0 && velocity.y != 0) {
            float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
            velocity.x = (velocity.x / length) * PLAYER_SPEED;
            velocity.y = (velocity.y / length) * PLAYER_SPEED;
        }
    }
}

void Player::dash() {
    if (!canDash()) return;
    
    // Get dash direction from current velocity or last movement
    if (velocity.x != 0 || velocity.y != 0) {
        float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        dashDirection = sf::Vector2f(velocity.x / length, velocity.y / length);
    } else {
        dashDirection = sf::Vector2f(1, 0); // Default right
    }
    
    isDashing = true;
    dashTimer = DASH_DURATION;
    dashCooldownTimer = DASH_COOLDOWN;
}

void Player::update(float dt) {
    handleInput();
    
    // Update dash
    if (isDashing) {
        dashTimer -= dt;
        if (dashTimer <= 0) {
            isDashing = false;
        }
        position += dashDirection * DASH_SPEED * dt;
    } else {
        position += velocity * dt;
    }
    
    // Update cooldown
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;
    }
    
    // Keep player in bounds
    if (position.x < PLAYER_SIZE / 2) position.x = PLAYER_SIZE / 2;
    if (position.x > WINDOW_WIDTH - PLAYER_SIZE / 2) 
        position.x = WINDOW_WIDTH - PLAYER_SIZE / 2;
    if (position.y < PLAYER_SIZE / 2) position.y = PLAYER_SIZE / 2;
    if (position.y > WINDOW_HEIGHT - PLAYER_SIZE / 2) 
        position.y = WINDOW_HEIGHT - PLAYER_SIZE / 2;
    
    shape.setPosition(position);
    
    // Pulse effect during dash
    if (isDashing) {
        float scale = 1.0f + sin(dashTimer * 30) * 0.2f;
        shape.setScale(scale, scale);
    } else {
        shape.setScale(1.0f, 1.0f);
    }
}

void Player::drawTrail(sf::RenderWindow& window, ParticleSystem& particles) {
    trailTimer += 0.016f; // Approximate dt
    if (trailTimer >= 0.05f) {
        particles.emitTrail(position, currentColor);
        trailTimer = 0;
    }
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

void Player::changeColor() {
    // Cycle to next color
    currentColorIndex = (currentColorIndex + 1) % availableColors.size();
    currentColor = availableColors[currentColorIndex];
    shape.setFillColor(currentColor);
}

void Player::reset() {
    position = sf::Vector2f(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f);
    velocity = sf::Vector2f(0, 0);
    isDashing = false;
    dashTimer = 0;
    dashCooldownTimer = 0;
    currentColorIndex = 0;
    currentColor = availableColors[currentColorIndex];
    shape.setFillColor(currentColor);
}