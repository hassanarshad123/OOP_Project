#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "ParticleSystem.h"

class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color currentColor;
    
    // Dash mechanics
    bool isDashing;
    float dashTimer;
    float dashCooldownTimer;
    sf::Vector2f dashDirection;
    
    // Trail effect
    float trailTimer;
    
public:
    Player();
    
    // Movement
    void update(float dt);
    void handleInput();
    void dash();
    
    // Getters
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Color getColor() const { return currentColor; }
    bool canDash() const { return dashCooldownTimer <= 0 && !isDashing; }
    float getDashCooldown() const { return dashCooldownTimer; }
    
    // Drawing
    void draw(sf::RenderWindow& window);
    void drawTrail(sf::RenderWindow& window, ParticleSystem& particles);
    
    // Reset
    void reset();
};

#endif
