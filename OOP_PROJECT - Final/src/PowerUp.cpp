#include "PowerUp.h"
#include <cmath>

PowerUp::PowerUp(sf::Vector2f startPos, PowerUpType t, float speed) {
    position = startPos;
    type = t;
    isActive = true;
    pulseTimer = 0;
    
    shape.setRadius(15.0f);
    shape.setOrigin(15.0f, 15.0f);
    shape.setOutlineThickness(3.0f);
    shape.setOutlineColor(sf::Color::White);
    
    // Set color based on type
    switch(type) {
        case PowerUpType::SHIELD:
            shape.setFillColor(COLOR_GREEN);
            break;
        case PowerUpType::SLOW_TIME:
            shape.setFillColor(COLOR_PURPLE);
            break;
        case PowerUpType::SCORE_BOOST:
            shape.setFillColor(COLOR_YELLOW);
            break;
    }
    
    velocity = sf::Vector2f(-speed, 0);
}

void PowerUp::update(float dt) {
    if (!isActive) return;
    
    position += velocity * dt;
    pulseTimer += dt;
    
    // Pulsing effect
    float scale = 1.0f + sin(pulseTimer * 5) * 0.2f;
    shape.setScale(scale, scale);
    
    shape.setPosition(position);
    
    // Deactivate if off screen
    if (position.x < -30) {
        isActive = false;
    }
}

void PowerUp::draw(sf::RenderWindow& window) {
    if (isActive) {
        // Draw with glow
        sf::RenderStates states;
        states.blendMode = sf::BlendAdd;
        window.draw(shape, states);
        
        // Draw normal too for solid part
        states.blendMode = sf::BlendAlpha;
        window.draw(shape, states);
    }
}
