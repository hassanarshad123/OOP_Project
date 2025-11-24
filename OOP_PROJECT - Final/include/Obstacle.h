#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include "Config.h"

class Obstacle {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    bool isActive;
    float rotation;
    float rotationSpeed;

public:
    Obstacle(sf::Vector2f startPos, sf::Color col, float speed);
    
    // Update
    void update(float dt);
    
    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool active() const { return isActive; }
    sf::Color getColor() const { return color; }
    
    // Set inactive
    void deactivate() { isActive = false; }
    
    // Drawing
    void draw(sf::RenderWindow& window);
};

#endif
