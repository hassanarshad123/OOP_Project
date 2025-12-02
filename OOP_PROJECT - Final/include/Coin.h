#ifndef COIN_H
#define COIN_H

#include <SFML/Graphics.hpp>
#include "Config.h"

class Coin {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    float rotation;
    float rotationSpeed;
    bool isActive;
    int value;

public:
    Coin(sf::Vector2f startPos, float speed, int val);

    void update(float dt);
    void draw(sf::RenderWindow& window);

    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool active() const { return isActive; }
    void deactivate() { isActive = false; }
    int getValue() const { return value; }
};

#endif
