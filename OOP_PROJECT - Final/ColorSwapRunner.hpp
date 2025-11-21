#include <SFML/Graphics.hpp>
#include <vector>

// ==================== ENUMS ====================

enum ScreenType {
    SCREEN_WELCOME,
    SCREEN_GAME
};

enum PlayerColor {
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_YELLOW
};


// ==================== FORWARD DECLARATIONS ====================

class Game;

// ==================== INTERFACE ====================

class IScreen {
public:
    virtual ~IScreen() {}
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

// ==================== PLAYER ====================

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
    float getX() const;
};

// ==================== OBSTACLE ====================

class Obstacle {
private:
    sf::RectangleShape shape;
    float speed;
    float resetX;
    PlayerColor color;
    bool hasCounted;

    void updateColor();

public:
    Obstacle(float x, float y, float width, float height, float speed, PlayerColor color);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getBounds() const;
    void reset();
    void setColor(PlayerColor newColor);
    bool isOffScreen() const;
    PlayerColor getColor() const;
    bool shouldCount(float playerX);
    float getX() const;
};

// ==================== LEVEL MANAGER ====================

class LevelManager {
private:
    std::vector<Obstacle*> obstacles;
    float spawnTimer;
    float spawnInterval;
    int obstaclesPassed;
    int colorIndex;
    int heightIndex;


    PlayerColor getRandomColor();
    float getRandomHeight();

public:
    LevelManager();
    ~LevelManager();
    void update(float dt, float playerX);
    void draw(sf::RenderWindow& window);
    void checkCollisions(const sf::FloatRect& playerBounds, PlayerColor playerColor, bool& collision);
    void reset();
    int getScore() const;
};

// ==================== WELCOME SCREEN ====================

class WelcomeScreen : public IScreen {
private:
    sf::Text titleText;
    sf::Text hintText;
    sf::Text controlsText;
    Game* game;

public:
    WelcomeScreen(const sf::Font& font, Game* gamePtr);
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void draw(sf::RenderWindow& window);
};

// ==================== GAME SCREEN ====================

class GameScreen : public IScreen {
private:
    Player player;
    LevelManager levelManager;
    sf::Text scoreText;
    sf::Text gameOverText;
    sf::Text instructionText;
    Game* game;
    bool isGameOver;

public:
    GameScreen(const sf::Font& font, Game* gamePtr);
    void handleEvent(const sf::Event& event);
    void update(float dt);
    void draw(sf::RenderWindow& window);
    void reset();
    void checkCollision();
};

// ==================== GAME ====================

class Game {
private:
    sf::RenderWindow window;
    sf::Font font;
    IScreen* currentScreen;
    sf::Clock clock;

    void loadFont();
    void createScreen(ScreenType type);

public:
    Game();
    ~Game();
    void run();
    void switchScreen(ScreenType type);
};
