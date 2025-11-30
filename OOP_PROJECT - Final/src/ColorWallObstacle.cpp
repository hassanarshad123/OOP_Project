#include "ColorWallObstacle.h"

ColorWallObstacle::ColorWallObstacle(sf::Vector2f startPos, sf::Color col, float speed)
    : Obstacle(startPos, col, speed) {  // Call parent constructor

    requiredColor = col;
    isTall = true;
    shape.setSize(sf::Vector2f(OBSTACLE_WIDTH * 3, WINDOW_HEIGHT * 0.8f));
    shape.setOrigin(OBSTACLE_WIDTH * 1.5f, WINDOW_HEIGHT * 0.4f);
    shape.setFillColor(requiredColor);


    shape.setOutlineThickness(5.0f);
    shape.setOutlineColor(sf::Color::White);
    rotationSpeed = 0.0f;
}

void ColorWallObstacle::draw(sf::RenderWindow& window) {
    if (!active()) return;

    sf::RectangleShape glow = shape;
    glow.setFillColor(sf::Color(requiredColor.r, requiredColor.g, requiredColor.b, 100));
    glow.setOutlineThickness(10.0f);
    glow.setOutlineColor(sf::Color(requiredColor.r, requiredColor.g, requiredColor.b, 50));
    window.draw(glow);

    // Draw the main wall
    window.draw(shape);
}
