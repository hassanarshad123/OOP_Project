#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f startPos, sf::Color col, float speed) {
    position = startPos;
    color = col;
    isActive = true;
    rotation = 0;
    rotationSpeed = 180.0f;
    
    shape.setSize(sf::Vector2f(OBSTACLE_WIDTH, OBSTACLE_HEIGHT));
    shape.setOrigin(OBSTACLE_WIDTH / 2, OBSTACLE_HEIGHT / 2);
    shape.setFillColor(color);
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::White);
    
    velocity = sf::Vector2f(-speed, 0);
}

void Obstacle::update(float dt) {
    if (!isActive) return;
    
    position += velocity * dt;
    rotation += rotationSpeed * dt;
    
    shape.setPosition(position);
    shape.setRotation(rotation);
    
    // Deactivate if off screen
    if (position.x < -OBSTACLE_WIDTH) {
        isActive = false;
    }
}

void Obstacle::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}
