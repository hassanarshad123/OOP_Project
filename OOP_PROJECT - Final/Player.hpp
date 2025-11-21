#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

enum class PlayerColor {
    Red,
    Green,
    Blue,
    Yellow
};

class Player {
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    float gravity;
    float jumpStrength;
    float moveSpeed;
    bool isOnGround;
    float groundLevel;
    PlayerColor currentColor;
    bool canSwapColor;
    sf::Clock swapCooldownClock;

    void updateColor();

public:
    Player(float x, float y, float size);
    void handleInput();
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void reset(float x, float y);
    PlayerColor getColor() const;
    void swapColor();
};

#endif