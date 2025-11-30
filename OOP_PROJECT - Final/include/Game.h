#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Config.h"
#include "Player.h"
#include "Obstacle.h"
#include "ColorWallObstacle.h"  // Include the new color wall class
#include "PowerUp.h"
#include "ParticleSystem.h"
#include "UIManager.h"

enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

class Game {
private:
    sf::RenderWindow window;
    GameState state;
    
    // Game objects
    Player player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<PowerUp>> powerUps;
    ParticleSystem particles;
    UIManager ui;
    
    // Game stats
    int score;
    int combo;
    float obstacleSpawnTimer;
    float currentObstacleSpeed;
    float currentSpawnTime;
    float powerUpSpawnTimer;
    float colorWallSpawnTimer;  // Timer for spawning color walls
    
    // Screen shake
    float shakeIntensity;
    float shakeTimer;
    sf::Vector2f cameraOffset;
    
    // Background
    std::vector<sf::CircleShape> backgroundStars;

    // Sound effects
    sf::SoundBuffer dashBuffer;
    sf::Sound dashSound;

    sf::SoundBuffer wallPassBuffer;
    sf::Sound wallPassSound;

    // Background music
    sf::Music backgroundMusic;

public:
    Game();
    
    // Main game loop
    void run();
    
private:
    // Game loop components
    void processEvents();
    void update(float dt);
    void render();
    
    // State management
    void startGame();
    void resetGame();
    void gameOver();
    
    // Game logic
    void spawnObstacle();
    void spawnPowerUp();
    void spawnColorWall();  // Spawn special color wall obstacles
    void checkCollisions();
    void updateDifficulty();
    void screenShake(float intensity);
    
    // Helpers
    sf::Color getRandomColor();
    void createBackground();
};

#endif
