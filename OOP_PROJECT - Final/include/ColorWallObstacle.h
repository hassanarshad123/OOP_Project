#ifndef COLORWALLOBSTACLE_H
#define COLORWALLOBSTACLE_H

#include "Obstacle.h"
#include "Config.h"

// ColorWallObstacle inherits from Obstacle (INHERITANCE - OOP Concept)
// This is a special type of obstacle that requires color matching
class ColorWallObstacle : public Obstacle {
private:
    sf::Color requiredColor;  // Color player must match to pass through
    bool isTall;              // This obstacle is taller than regular ones

public:
    // Constructor
    ColorWallObstacle(sf::Vector2f startPos, sf::Color col, float speed);

    // Override the draw method to make it look different
    void draw(sf::RenderWindow& window) override;

    // Getter for required color
    sf::Color getRequiredColor() const { return requiredColor; }

    // Check if this is a color wall (useful for collision detection)
    bool isColorWall() const { return true; }
};

#endif
