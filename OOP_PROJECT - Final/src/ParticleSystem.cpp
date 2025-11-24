#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
    std::random_device rd;
    rng.seed(rd());
}

void ParticleSystem::emit(sf::Vector2f position, sf::Color color, int count) {
    for (int i = 0; i < count; i++) {
        Particle p;
        p.position = position;
        
        // Random direction
        float angle = random(0, 6.28318f); // 2 * PI
        float speed = random(50, 200);
        p.velocity.x = cos(angle) * speed;
        p.velocity.y = sin(angle) * speed;
        
        p.color = color;
        p.lifetime = random(0.5f, 1.5f);
        p.size = random(2.0f, 6.0f);
        
        particles.push_back(p);
    }
}

void ParticleSystem::emitTrail(sf::Vector2f position, sf::Color color) {
    // Small trail particles
    for (int i = 0; i < 3; i++) {
        Particle p;
        p.position = position + sf::Vector2f(random(-5, 5), random(-5, 5));
        p.velocity = sf::Vector2f(random(-30, 30), random(-30, 30));
        p.color = color;
        p.lifetime = random(0.2f, 0.5f);
        p.size = random(3.0f, 7.0f);
        particles.push_back(p);
    }
}

void ParticleSystem::emitExplosion(sf::Vector2f position, sf::Color color) {
    // Big explosion
    emit(position, color, 30);
}

void ParticleSystem::update(float dt) {
    // Update and remove dead particles
    for (auto it = particles.begin(); it != particles.end();) {
        it->lifetime -= dt;
        
        if (it->lifetime <= 0) {
            it = particles.erase(it);
        } else {
            // Update position
            it->position += it->velocity * dt;
            
            // Fade out
            float alpha = (it->lifetime / 1.5f) * 255;
            it->color.a = static_cast<sf::Uint8>(alpha);
            
            ++it;
        }
    }
}

void ParticleSystem::draw(sf::RenderWindow& window) {
    for (const auto& p : particles) {
        sf::CircleShape shape(p.size);
        shape.setPosition(p.position);
        shape.setFillColor(p.color);
        shape.setOrigin(p.size, p.size);
        
        // Draw with additive blending for glow
        sf::RenderStates states;
        states.blendMode = sf::BlendAdd;
        window.draw(shape, states);
    }
}

void ParticleSystem::clear() {
    particles.clear();
}

float ParticleSystem::random(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}
