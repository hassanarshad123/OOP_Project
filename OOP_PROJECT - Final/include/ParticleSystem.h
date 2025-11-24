#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <random>

// Simple particle struct
struct Particle {
    sf::Vector2f position;
    sf::Vector2f velocity;
    sf::Color color;
    float lifetime;
    float size;
};

class ParticleSystem {
private:
    std::vector<Particle> particles;
    std::mt19937 rng;

public:
    ParticleSystem();
    
    // Emit particles
    void emit(sf::Vector2f position, sf::Color color, int count = 10);
    void emitTrail(sf::Vector2f position, sf::Color color);
    void emitExplosion(sf::Vector2f position, sf::Color color);
    
    // Update particles
    void update(float dt);
    
    // Draw particles
    void draw(sf::RenderWindow& window);
    
    // Clear all particles
    void clear();

private:
    float random(float min, float max);
};

#endif
