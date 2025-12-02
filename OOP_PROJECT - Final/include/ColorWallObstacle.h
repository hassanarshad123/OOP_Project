#ifndef COLORWALLOBSTACLE_H
#define COLORWALLOBSTACLE_H

#include "Obstacle.h"
#include "Config.h"

class ColorWallObstacle : public Obstacle {
private:
    sf::Color requiredColor; 
    bool isTall;            

public:
    ColorWallObstacle(sf::Vector2f startPos, sf::Color col, float speed);
    

    void draw(sf::RenderWindow& window) override;

    // Getter for required color
    sf::Color getRequiredColor() const { return requiredColor; }

    // Check if this is a color wall (useful for collision detection)
    bool isColorWall() const { return true; }
};

#endif
