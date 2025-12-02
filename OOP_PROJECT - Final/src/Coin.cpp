#include "Coin.h"

Coin::Coin(sf::Vector2f startPos, float speed, int val) {
    position = startPos;
    value = val;
    isActive = true;
    rotation = 0;
    rotationSpeed = 360.0f;

    shape.setRadius(15.0f);
    shape.setOrigin(15.0f, 15.0f);
    shape.setFillColor(sf::Color::Yellow);
    shape.setOutlineThickness(30.0f);
    shape.setOutlineColor(sf::Color(255, 215, 0)); // Gold

    velocity = sf::Vector2f(-speed, 0);
}

void Coin::update(float dt) {
    if (!isActive) return;

    position += velocity * dt;
    rotation += rotationSpeed * dt;

    shape.setPosition(position);
    shape.setRotation(rotation);

    // Deactivate if off screen
    if (position.x < -30) {
        isActive = false;
    }
}

void Coin::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}
