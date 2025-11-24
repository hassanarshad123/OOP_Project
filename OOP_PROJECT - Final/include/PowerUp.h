#ifndef POWERUP_H
#define POWERUP_H

#include <SFML/Graphics.hpp>
#include "Config.h"

enum class PowerUpType {
    SHIELD,
    SLOW_TIME,
    SCORE_BOOST
};

class PowerUp {
private:
    sf::CircleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    PowerUpType type;
    bool isActive;
    float pulseTimer;

public:
    PowerUp(sf::Vector2f startPos, PowerUpType t, float speed);
    
    // Update
    void update(float dt);
    
    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool active() const { return isActive; }
    PowerUpType getType() const { return type; }
    
    // Set inactive
    void deactivate() { isActive = false; }
    
    // Drawing
    void draw(sf::RenderWindow& window);
};

#endif
