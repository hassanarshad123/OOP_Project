#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include "Config.h"

class Obstacle {
protected:  // Changed to protected so child classes can access these
    sf::RectangleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    bool isActive;
    float rotation;
    float rotationSpeed;

public:
    Obstacle(sf::Vector2f startPos, sf::Color col, float speed);
    virtual ~Obstacle() {}  // Virtual destructor for proper inheritance

    // Update
    virtual void update(float dt);  // Virtual so child classes can override

    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool active() const { return isActive; }
    sf::Color getColor() const { return color; }

    // Set inactive
    void deactivate() { isActive = false; }

    // Drawing - virtual so child classes can override
    virtual void draw(sf::RenderWindow& window);

    // Virtual method to check if this is a color wall
    virtual bool isColorWall() const { return false; }
};

#endif
