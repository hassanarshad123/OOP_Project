#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>

// Window settings
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const std::string WINDOW_TITLE = "Neon Dodge - Color Rush";
const int FPS = 60;

// Player settings
const float PLAYER_SIZE = 40.0f;
const float PLAYER_SPEED = 350.0f;
const float DASH_SPEED = 800.0f;
const float DASH_DURATION = 0.2f;
const float DASH_COOLDOWN = 1.0f;

// Game settings
const float OBSTACLE_SPAWN_TIME = 1.5f;
const float OBSTACLE_SPEED = 300.0f;
const float OBSTACLE_WIDTH = 40.0f;
const float OBSTACLE_HEIGHT = 40.0f;
const float SPEED_INCREASE_RATE = 0.95f;

// Colors - Neon theme
const sf::Color COLOR_RED = sf::Color(255, 0, 100);
const sf::Color COLOR_BLUE = sf::Color(0, 200, 255);
const sf::Color COLOR_YELLOW = sf::Color(255, 255, 0);
const sf::Color COLOR_GREEN = sf::Color(0, 255, 150);
const sf::Color COLOR_PURPLE = sf::Color(200, 0, 255);
const sf::Color COLOR_ORANGE = sf::Color(255, 150, 0);
const sf::Color COLOR_BACKGROUND = sf::Color(10, 10, 30);

// Scoring
const int SCORE_PER_DODGE = 10;
const int SCORE_POWERUP = 50;
const int COMBO_THRESHOLD = 5;

#endif
