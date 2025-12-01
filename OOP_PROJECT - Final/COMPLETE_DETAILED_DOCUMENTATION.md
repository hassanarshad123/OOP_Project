# COMPLETE PROJECT DOCUMENTATION
## Color Swap Runner 2.0 - OOP Final Project

**For Team Members & Teacher Presentation**

---

# TABLE OF CONTENTS

1. [Project Overview](#1-project-overview)
2. [OOP Concepts - Complete Explanation](#2-oop-concepts---complete-explanation)
3. [File-by-File Complete Analysis](#3-file-by-file-complete-analysis)
4. [Game Mechanics & Logic](#4-game-mechanics--logic)
5. [Code Flow & Execution](#5-code-flow--execution)
6. [Class Relationships & Architecture](#6-class-relationships--architecture)
7. [Common Questions & Answers](#7-common-questions--answers)
8. [Technical Implementation Details](#8-technical-implementation-details)

---

# 1. PROJECT OVERVIEW

## 1.1 What Is This Game?

**Color Swap Runner 2.0** is a 2D arcade-style endless runner game where:
- Player controls a colored square
- Obstacles come from the right side of screen
- Player must dodge regular obstacles OR match color with color walls
- Score increases as you survive longer
- Difficulty progressively increases

## 1.2 Core Gameplay Loop

```
START GAME
    ↓
MENU (Press Enter)
    ↓
PLAYING STATE
    ↓
├─ Regular obstacles spawn → Dodge them
├─ Color walls spawn → Match color (press C) and pass through
├─ Power-ups spawn → Collect for points
├─ Player dashes (Space) → Avoid danger
├─ Score increases → Difficulty increases
    ↓
COLLISION (wrong color or hit obstacle)
    ↓
GAME OVER (Press Enter to restart)
```

## 1.3 Controls

| Key | Action |
|-----|--------|
| WASD / Arrow Keys | Move player |
| Space | Dash (high-speed movement, 1s cooldown) |
| C | Change color (cycles through 6 colors) |
| Enter | Start game / Restart after game over |
| Escape | Quit game |

## 1.4 Technologies Used

- **Language:** C++17
- **Graphics Library:** SFML 2.5 (Simple and Fast Multimedia Library)
- **Compiler:** GCC (MinGW)
- **Platform:** Windows
- **Total Lines of Code:** ~1200 lines
- **Total Classes:** 8 classes

---

# 2. OOP CONCEPTS - COMPLETE EXPLANATION

## 2.1 The Four Pillars of OOP (Used in This Project)

### PILLAR 1: ENCAPSULATION

**Definition:** Bundling data (variables) and methods (functions) that operate on that data within a single unit (class), while hiding internal details from outside access.

**Why It Matters:** Protects data from being accidentally modified, makes code more maintainable.

**Example in Our Project:**

```cpp
// In Player.h
class Player {
private:  // Hidden from outside
    sf::Vector2f position;
    sf::Vector2f velocity;
    int currentColorIndex;
    std::vector<sf::Color> availableColors;

public:  // Public interface
    void changeColor();  // Controlled way to change color
    sf::Color getColor() const;  // Controlled way to get color
};
```

**Teacher Question:** "Why is position private?"
**Answer:** "If position was public, any code could change it directly like `player.position = sf::Vector2f(9999, 9999)`, which could break the game. By making it private and providing controlled methods, we ensure the player position is always valid (stays within screen bounds)."

---

### PILLAR 2: INHERITANCE

**Definition:** A mechanism where a new class (child/derived) inherits properties and methods from an existing class (parent/base).

**Why It Matters:** Code reuse, establishes "IS-A" relationship, reduces duplication.

**Example in Our Project:**

```cpp
// Parent class (Obstacle.h)
class Obstacle {
protected:  // Accessible to child classes
    sf::RectangleShape shape;
    sf::Vector2f position;
    sf::Color color;

public:
    virtual void draw(sf::RenderWindow& window);
    virtual bool isColorWall() const { return false; }
};

// Child class (ColorWallObstacle.h)
class ColorWallObstacle : public Obstacle {
private:
    sf::Color requiredColor;

public:
    ColorWallObstacle(sf::Vector2f pos, sf::Color col, float speed);
    void draw(sf::RenderWindow& window) override;  // Different implementation
    bool isColorWall() const override { return true; }  // Different behavior
};
```

**Teacher Question:** "What does ColorWallObstacle inherit from Obstacle?"
**Answer:** "It inherits all member variables (shape, position, velocity, color, isActive, rotation) and all methods (update, getPosition, getBounds, etc.). This means we don't have to rewrite all that code - we get it for free! We only need to write the parts that are different for color walls."

**IS-A Relationship:** "A ColorWallObstacle IS-A Obstacle. Anything an Obstacle can do, a ColorWallObstacle can also do."

---

### PILLAR 3: POLYMORPHISM

**Definition:** The ability to treat objects of different classes through the same interface, where each class can provide its own implementation.

**Why It Matters:** Flexible code that can work with different object types without knowing their exact type.

**Example in Our Project:**

```cpp
// In Game.h - ONE vector holds BOTH types
std::vector<std::unique_ptr<Obstacle>> obstacles;

// In Game.cpp - Can add BOTH types to same vector
void Game::spawnObstacle() {
    obstacles.push_back(std::make_unique<Obstacle>(...));
}

void Game::spawnColorWall() {
    obstacles.push_back(std::make_unique<ColorWallObstacle>(...));
}

// When we loop through, each calls its OWN version of draw()
for (const auto& obstacle : obstacles) {
    obstacle->draw(window);  // Calls Obstacle::draw() OR ColorWallObstacle::draw()
    if (obstacle->isColorWall()) {  // Returns false OR true depending on type
        // Special handling for color walls
    }
}
```

**Teacher Question:** "How does the computer know which draw() to call?"
**Answer:** "This is called dynamic dispatch or runtime polymorphism. Because draw() is marked as 'virtual' in the base class, the computer uses a virtual function table (vtable) to look up the correct version at runtime based on the actual object type. Even though we're using a pointer to Obstacle, if the actual object is a ColorWallObstacle, it calls ColorWallObstacle::draw()."

---

### PILLAR 4: ABSTRACTION

**Definition:** Hiding complex implementation details and showing only essential features through a simple interface.

**Why It Matters:** Makes complex systems easier to use and understand.

**Example in Our Project:**

```cpp
// Simple interface
class ParticleSystem {
public:
    void emit(sf::Vector2f position, sf::Color color, int count);
    void emitTrail(sf::Vector2f position, sf::Color color);
    void emitExplosion(sf::Vector2f position, sf::Color color);
};

// Complex hidden implementation
private:
    std::vector<Particle> particles;  // User doesn't need to know this exists

    void update(float dt) {
        // Complex physics calculations
        // Lifetime management
        // Alpha blending math
        // User doesn't need to know any of this!
    }
};

// Usage - Simple!
particles.emitExplosion(player.getPosition(), COLOR_RED);
// That one line creates 30 particles, calculates velocities, manages lifetimes, etc.
```

**Teacher Question:** "Why abstract the particle system?"
**Answer:** "The game code doesn't need to know HOW particles work internally. It just needs to say 'create explosion here' and the ParticleSystem handles all the complex math. This separation of concerns makes the code easier to maintain - if we want to change how particles work, we only change one class."

---

## 2.2 Additional OOP Concepts Used

### COMPOSITION

**Definition:** Building complex objects by combining simpler objects (HAS-A relationship).

**Example:**

```cpp
class Game {
private:
    Player player;                    // Game HAS-A Player
    ParticleSystem particles;         // Game HAS-A ParticleSystem
    UIManager ui;                     // Game HAS-A UIManager
    std::vector<std::unique_ptr<Obstacle>> obstacles;  // Game HAS-A collection of Obstacles
};
```

**Teacher Question:** "Why use composition instead of inheritance?"
**Answer:** "Because Game doesn't need to 'be' a Player or 'be' a ParticleSystem - it needs to 'have' them. The relationship is ownership, not specialization. Composition is more flexible than inheritance because we can easily swap components or add new ones without changing the entire class hierarchy."

---

### ACCESS MODIFIERS

**Three levels of access control:**

1. **private:** Only accessible within the class itself
2. **protected:** Accessible within the class AND its children
3. **public:** Accessible from anywhere

**Example:**

```cpp
class Obstacle {
protected:  // Changed from private for inheritance
    sf::RectangleShape shape;  // Children can access this
    sf::Vector2f position;     // Children can access this

private:
    // Nothing here - but if we had private members, only Obstacle could access them

public:
    virtual void draw(sf::RenderWindow& window);  // Anyone can call this
};
```

**Teacher Question:** "Why protected instead of private?"
**Answer:** "We changed from private to protected when we added inheritance. ColorWallObstacle needs to access 'shape' to modify it (making it taller). If it was private, the child class couldn't access it. Protected means 'private to the outside world, but accessible to my children'."

---

### VIRTUAL FUNCTIONS & OVERRIDE

**Virtual:** Tells compiler "this method can be overridden by child classes"
**Override:** Tells compiler "I'm replacing the parent's version"

```cpp
// Parent
class Obstacle {
public:
    virtual void draw(sf::RenderWindow& window);  // CAN be overridden
    virtual bool isColorWall() const { return false; }  // CAN be overridden
    virtual ~Obstacle() {}  // Virtual destructor (important for polymorphism)
};

// Child
class ColorWallObstacle : public Obstacle {
public:
    void draw(sf::RenderWindow& window) override;  // DOES override
    bool isColorWall() const override { return true; }  // DOES override
};
```

**Teacher Question:** "What happens if we don't use virtual?"
**Answer:** "Without virtual, we get static binding - the method called is determined at compile time based on the pointer type. With our `Obstacle*` pointer, it would always call Obstacle::draw() even if the object is really a ColorWallObstacle. Virtual enables dynamic binding - the actual method is chosen at runtime based on the real object type."

---

### CONST CORRECTNESS

**const methods:** Promise not to modify the object

```cpp
class Player {
public:
    sf::Vector2f getPosition() const { return position; }  // Doesn't modify player
    sf::Color getColor() const { return currentColor; }    // Doesn't modify player

    void changeColor() { /* Modifies player, so NOT const */ }
    void update(float dt) { /* Modifies player, so NOT const */ }
};
```

**Teacher Question:** "Why bother with const?"
**Answer:** "Three reasons: 1) Documentation - it tells other programmers this method won't change the object. 2) Safety - the compiler prevents accidental modifications. 3) Allows use in const contexts - if we have a const Obstacle&, we can only call const methods on it."

---

### SMART POINTERS (Modern C++ Memory Management)

**std::unique_ptr:** Exclusive ownership, automatic deletion

```cpp
// Old C++ (BAD - memory leaks!)
Obstacle* obs = new Obstacle(...);
delete obs;  // Easy to forget!

// Modern C++ (GOOD - automatic cleanup!)
std::unique_ptr<Obstacle> obs = std::make_unique<Obstacle>(...);
// Automatically deleted when obs goes out of scope

// In our project:
std::vector<std::unique_ptr<Obstacle>> obstacles;
obstacles.push_back(std::make_unique<ColorWallObstacle>(...));
// When obstacle is removed from vector, memory is automatically freed!
```

**Teacher Question:** "Why unique_ptr instead of raw pointers?"
**Answer:** "Memory safety! With raw pointers, we have to manually delete objects, which is error-prone and can cause memory leaks or double-deletion crashes. unique_ptr automatically deletes the object when it's no longer needed. It also makes ownership clear - there's exactly ONE owner of each object."

---

## 2.3 Design Patterns Used

### GAME LOOP PATTERN

```cpp
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        float dt = clock.restart().asSeconds();  // Delta time
        processEvents();  // Input
        update(dt);       // Logic
        render();         // Draw
    }
}
```

**Teacher Question:** "Why separate processEvents, update, and render?"
**Answer:** "Separation of concerns. Each method has ONE responsibility: processEvents handles input, update handles game logic, render handles drawing. This makes the code easier to understand, test, and modify. It also follows the Single Responsibility Principle from SOLID design principles."

---

### STATE PATTERN

```cpp
enum class GameState {
    MENU,
    PLAYING,
    GAME_OVER
};

// Different behavior based on state
if (state == GameState::PLAYING) {
    // Update game logic
} else if (state == GameState::MENU) {
    // Show menu
}
```

**Teacher Question:** "Why use an enum for states?"
**Answer:** "Type safety and clarity. We could use integers (0=menu, 1=playing), but enum class makes the code self-documenting and prevents invalid states. The compiler ensures we can only use MENU, PLAYING, or GAME_OVER - we can't accidentally set state to 99."

---

### FACTORY PATTERN (Simplified)

```cpp
void Game::spawnObstacle() {
    // Factory method - creates objects
    sf::Vector2f pos(WINDOW_WIDTH + OBSTACLE_WIDTH, randomY);
    obstacles.push_back(std::make_unique<Obstacle>(pos, color, speed));
}

void Game::spawnColorWall() {
    // Another factory method
    obstacles.push_back(std::make_unique<ColorWallObstacle>(pos, color, speed));
}
```

**Teacher Question:** "Why factory methods?"
**Answer:** "Centralized object creation. All obstacle creation happens in these methods, so if we need to change how obstacles are created (different positioning, validation, etc.), we only change it in one place. This follows the DRY principle (Don't Repeat Yourself)."

---

# 3. FILE-BY-FILE COMPLETE ANALYSIS

## FILE 1: Config.h

**Purpose:** Central configuration file for all game constants
**Location:** `include/Config.h`
**Type:** Header-only (no .cpp file)
**Lines of Code:** 48

### Complete Code Breakdown:

```cpp
#ifndef CONFIG_H  // Include guard - prevents multiple inclusion
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <string>

// ============================================
// WINDOW SETTINGS
// ============================================
const int WINDOW_WIDTH = 1280;   // Screen width in pixels
const int WINDOW_HEIGHT = 720;   // Screen height in pixels
const std::string WINDOW_TITLE = "Color Swap Runner(Upgraded)";
const int FPS = 60;  // Frames per second limit

// ============================================
// PLAYER SETTINGS
// ============================================
const float PLAYER_SIZE = 50.0f;          // Square size (50x50 pixels)
const float PLAYER_SPEED = 350.0f;        // Movement speed (pixels per second)
const float DASH_SPEED = 800.0f;          // Dash speed (2.28x faster)
const float DASH_DURATION = 0.2f;         // Dash lasts 0.2 seconds
const float DASH_COOLDOWN = 1.0f;         // 1 second cooldown between dashes

// ============================================
// OBSTACLE SETTINGS
// ============================================
const float OBSTACLE_SPAWN_TIME = 1.5f;   // Spawn obstacle every 1.5 seconds
const float OBSTACLE_SPEED = 300.0f;      // Initial speed (pixels per second)
const float OBSTACLE_WIDTH = 40.0f;       // Regular obstacle size
const float OBSTACLE_HEIGHT = 40.0f;
const float SPEED_INCREASE_RATE = 0.95f;  // Speed multiplier (5% faster each time)

// ============================================
// COLOR WALL SETTINGS
// ============================================
const float COLOR_WALL_SPAWN_TIME = 8.0f; // Spawn color wall every 8 seconds
const int SCORE_COLOR_WALL_PASS = 50;     // Bonus points for passing color wall

// ============================================
// COLORS (Neon Theme)
// ============================================
const sf::Color COLOR_RED = sf::Color(255, 0, 100);      // Neon red
const sf::Color COLOR_BLUE = sf::Color(0, 200, 255);     // Neon blue
const sf::Color COLOR_YELLOW = sf::Color(255, 255, 0);   // Bright yellow
const sf::Color COLOR_GREEN = sf::Color(0, 255, 150);    // Neon green
const sf::Color COLOR_PURPLE = sf::Color(200, 0, 255);   // Neon purple
const sf::Color COLOR_ORANGE = sf::Color(255, 150, 0);   // Neon orange
const sf::Color COLOR_BACKGROUND = sf::Color(10, 10, 30); // Dark blue background

// ============================================
// SCORING
// ============================================
const int SCORE_PER_DODGE = 10;     // Points for dodging regular obstacle
const int SCORE_POWERUP = 50;       // Points for collecting power-up
const int COMBO_THRESHOLD = 5;      // Show combo when >= 5 consecutive dodges

#endif
```

### Why This Design?

**Teacher Question:** "Why a separate config file?"
**Answer:**
1. **Single Source of Truth:** All constants in one place
2. **Easy Balancing:** Change game difficulty without touching game logic
3. **Prevents Magic Numbers:** No random numbers scattered in code
4. **Compile-Time Constants:** const means these are baked in at compile time (fast!)
5. **Type Safety:** const int vs #define gives us type checking

**Teacher Question:** "Why const instead of #define?"
**Answer:**
```cpp
// #define (C-style - BAD)
#define PLAYER_SIZE 50.0f  // No type checking, no scope, hard to debug

// const (C++ style - GOOD)
const float PLAYER_SIZE = 50.0f;  // Type-safe, scoped, debuggable
```

---

## FILE 2: Player.h

**Purpose:** Player character class declaration
**Location:** `include/Player.h`
**Type:** Header file
**Lines of Code:** 50

### Complete Code with Explanations:

```cpp
#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include "Config.h"
#include "ParticleSystem.h"

class Player {
private:
    // ============================================
    // VISUAL REPRESENTATION
    // ============================================
    sf::RectangleShape shape;  // The square that represents the player on screen

    // ============================================
    // PHYSICS PROPERTIES
    // ============================================
    sf::Vector2f position;     // Current position (x, y) in pixels
    sf::Vector2f velocity;     // Movement speed per second

    // ============================================
    // COLOR SWAPPING SYSTEM
    // ============================================
    sf::Color currentColor;                 // Currently displayed color
    int currentColorIndex;                  // Index in availableColors vector (0-5)
    std::vector<sf::Color> availableColors; // All 6 possible colors

    // ============================================
    // DASH ABILITY
    // ============================================
    bool isDashing;              // True when dash is active
    float dashTimer;             // Time remaining in current dash
    float dashCooldownTimer;     // Time until dash is available again
    sf::Vector2f dashDirection;  // Direction of dash movement

    // ============================================
    // VISUAL EFFECTS
    // ============================================
    float trailTimer;  // Controls particle trail emission rate

public:
    // ============================================
    // CONSTRUCTOR
    // ============================================
    Player();  // Initialize player at starting position

    // ============================================
    // MOVEMENT METHODS
    // ============================================
    void update(float dt);      // Update position, dash, timers (called every frame)
    void handleInput();         // Process WASD/Arrow key input
    void dash();                // Activate dash ability

    // ============================================
    // COLOR SWAPPING
    // ============================================
    void changeColor();  // Cycle to next color (called when C is pressed)

    // ============================================
    // GETTERS (const methods - don't modify player)
    // ============================================
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    sf::Color getColor() const { return currentColor; }
    bool canDash() const { return dashCooldownTimer <= 0 && !isDashing; }
    float getDashCooldown() const { return dashCooldownTimer; }

    // ============================================
    // RENDERING
    // ============================================
    void draw(sf::RenderWindow& window);
    void drawTrail(sf::RenderWindow& window, ParticleSystem& particles);

    // ============================================
    // RESET
    // ============================================
    void reset();  // Return to starting state (called when game restarts)
};

#endif
```

### Design Decisions Explained:

**Teacher Question:** "Why are position and velocity separate?"
**Answer:** "Position is where the player IS, velocity is how fast they're MOVING. We need both because:
- Position: Used for drawing and collision detection
- Velocity: Used for movement calculations
- Formula: `position += velocity * dt`"

**Teacher Question:** "Why store both currentColor and currentColorIndex?"
**Answer:**
- `currentColorIndex`: Tells us which color is selected (0-5)
- `currentColor`: The actual SFML color value for fast access
- We could calculate currentColor from availableColors[currentColorIndex] every time, but storing it is faster"

**Teacher Question:** "Why separate handleInput() and update()?"
**Answer:** "Separation of concerns:
- `handleInput()`: Only reads keyboard state
- `update()`: Applies physics, updates timers, handles dash logic
- Makes code easier to test - we could replace handleInput() with AI without changing update()"

---

## FILE 3: Player.cpp

**Purpose:** Player class implementation
**Location:** `src/Player.cpp`
**Lines of Code:** 142

### Constructor - Detailed Breakdown:

```cpp
Player::Player() {
    // SET STARTING POSITION
    // Place player 1/4 across screen, middle height
    position = sf::Vector2f(WINDOW_WIDTH / 4.0f, WINDOW_HEIGHT / 2.0f);
    // Why 1/4? Gives player time to see obstacles coming from right

    // CREATE VISUAL SHAPE
    shape.setSize(sf::Vector2f(PLAYER_SIZE, PLAYER_SIZE));  // 50x50 square
    shape.setOrigin(PLAYER_SIZE / 2, PLAYER_SIZE / 2);      // Center origin for rotation
    // Why center origin? Makes rotation and positioning easier

    // INITIALIZE COLOR SYSTEM
    // Add all 6 colors to the vector
    availableColors.push_back(COLOR_RED);
    availableColors.push_back(COLOR_BLUE);
    availableColors.push_back(COLOR_YELLOW);
    availableColors.push_back(COLOR_GREEN);
    availableColors.push_back(COLOR_PURPLE);
    availableColors.push_back(COLOR_ORANGE);

    // Start with first color (red)
    currentColorIndex = 0;
    currentColor = availableColors[currentColorIndex];
    shape.setFillColor(currentColor);

    // VISUAL STYLING
    shape.setOutlineThickness(3.0f);
    shape.setOutlineColor(sf::Color::White);  // White border for visibility

    // INITIALIZE DASH SYSTEM
    isDashing = false;
    dashTimer = 0;
    dashCooldownTimer = 0;
    trailTimer = 0;
}
```

### handleInput() - Detailed Breakdown:

```cpp
void Player::handleInput() {
    // RESET VELOCITY
    // Start at zero, then add based on keys pressed
    velocity = sf::Vector2f(0, 0);

    // ONLY PROCESS INPUT IF NOT DASHING
    // During dash, player movement is controlled by dashDirection
    if (!isDashing) {
        // CHECK VERTICAL MOVEMENT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            velocity.y = -PLAYER_SPEED;  // Negative Y = up (screen coordinates)
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            velocity.y = PLAYER_SPEED;   // Positive Y = down
        }

        // CHECK HORIZONTAL MOVEMENT
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            velocity.x = -PLAYER_SPEED;  // Negative X = left
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            velocity.x = PLAYER_SPEED;   // Positive X = right
        }

        // NORMALIZE DIAGONAL MOVEMENT
        // Problem: Moving diagonally would be 1.414x faster (Pythagorean theorem)
        // Solution: Normalize the vector to maintain constant speed
        if (velocity.x != 0 && velocity.y != 0) {
            // Calculate vector length: √(x² + y²)
            float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

            // Normalize: divide by length, then multiply by desired speed
            velocity.x = (velocity.x / length) * PLAYER_SPEED;
            velocity.y = (velocity.y / length) * PLAYER_SPEED;

            // Now diagonal movement is same speed as cardinal movement!
        }
    }
}
```

**Teacher Question:** "Why normalize diagonal movement?"
**Answer:** "Without normalization:
- Moving right: velocity = (350, 0), speed = 350 pixels/sec
- Moving right+up: velocity = (350, -350), speed = √(350² + 350²) = 495 pixels/sec
- That's 41% faster! Unfair advantage.
After normalization, all directions move at exactly 350 pixels/sec."

### dash() - Detailed Breakdown:

```cpp
void Player::dash() {
    // CHECK IF DASH IS AVAILABLE
    if (!canDash()) return;  // Exit if on cooldown or already dashing

    // DETERMINE DASH DIRECTION
    // Use current movement direction if moving, otherwise default to right
    if (velocity.x != 0 || velocity.y != 0) {
        // Normalize current velocity to get direction
        float length = sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
        dashDirection = sf::Vector2f(velocity.x / length, velocity.y / length);
    } else {
        // Not moving? Dash right by default
        dashDirection = sf::Vector2f(1, 0);
    }

    // ACTIVATE DASH
    isDashing = true;
    dashTimer = DASH_DURATION;      // 0.2 seconds of dash
    dashCooldownTimer = DASH_COOLDOWN;  // 1.0 second until next dash
}
```

### update() - Detailed Breakdown:

```cpp
void Player::update(float dt) {
    // STEP 1: GET PLAYER INPUT
    handleInput();

    // STEP 2: APPLY MOVEMENT
    if (isDashing) {
        // UPDATE DASH TIMER
        dashTimer -= dt;
        if (dashTimer <= 0) {
            isDashing = false;  // Dash finished
        }

        // DASH MOVEMENT
        // Move in dash direction at DASH_SPEED (800 pixels/sec)
        position += dashDirection * DASH_SPEED * dt;

    } else {
        // NORMAL MOVEMENT
        // Move based on input velocity (350 pixels/sec)
        position += velocity * dt;
    }

    // STEP 3: UPDATE COOLDOWN
    if (dashCooldownTimer > 0) {
        dashCooldownTimer -= dt;  // Count down to zero
    }

    // STEP 4: BOUNDARY CHECKING
    // Keep player inside screen bounds
    if (position.x < PLAYER_SIZE / 2)
        position.x = PLAYER_SIZE / 2;  // Left edge
    if (position.x > WINDOW_WIDTH - PLAYER_SIZE / 2)
        position.x = WINDOW_WIDTH - PLAYER_SIZE / 2;  // Right edge
    if (position.y < PLAYER_SIZE / 2)
        position.y = PLAYER_SIZE / 2;  // Top edge
    if (position.y > WINDOW_HEIGHT - PLAYER_SIZE / 2)
        position.y = WINDOW_HEIGHT - PLAYER_SIZE / 2;  // Bottom edge

    // STEP 5: UPDATE VISUAL
    shape.setPosition(position);  // Move shape to new position

    // STEP 6: PULSING EFFECT DURING DASH
    if (isDashing) {
        // Sine wave creates smooth pulsing
        float scale = 1.0f + sin(dashTimer * 30) * 0.2f;
        shape.setScale(scale, scale);  // Grows and shrinks
    } else {
        shape.setScale(1.0f, 1.0f);  // Normal size
    }
}
```

**Teacher Question:** "What is dt and why use it?"
**Answer:** "dt = delta time = time since last frame (in seconds). We use it for frame-rate independence:

Without dt:
```cpp
position += velocity;  // Moves 350 pixels per FRAME
// At 60 FPS: 350 * 60 = 21,000 pixels/sec
// At 30 FPS: 350 * 30 = 10,500 pixels/sec
// Different speeds on different computers!
```

With dt:
```cpp
position += velocity * dt;  // Moves 350 pixels per SECOND
// At 60 FPS: 350 * (1/60) = 5.83 pixels/frame = 350 pixels/sec
// At 30 FPS: 350 * (1/30) = 11.67 pixels/frame = 350 pixels/sec
// Same speed everywhere!
```

### changeColor() - Detailed Breakdown:

```cpp
void Player::changeColor() {
    // CYCLE TO NEXT COLOR
    // Modulo operator (%) wraps around to 0 after reaching 5
    currentColorIndex = (currentColorIndex + 1) % availableColors.size();
    // Examples:
    // 0 → 1 → 2 → 3 → 4 → 5 → 0 → 1 → ...

    // UPDATE COLOR
    currentColor = availableColors[currentColorIndex];
    shape.setFillColor(currentColor);
}
```

---

## FILE 4: Obstacle.h

**Purpose:** Base obstacle class (regular obstacles)
**Location:** `include/Obstacle.h`
**Type:** Header file
**Lines of Code:** 41

### Complete Code with Explanations:

```cpp
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SFML/Graphics.hpp>
#include "Config.h"

class Obstacle {
protected:  // IMPORTANT: protected, not private!
    // Why protected? Child classes (ColorWallObstacle) need access

    sf::RectangleShape shape;  // Visual representation
    sf::Vector2f position;     // Current position
    sf::Vector2f velocity;     // Movement speed (always moving left)
    sf::Color color;           // Obstacle color
    bool isActive;             // false = should be deleted
    float rotation;            // Current rotation angle
    float rotationSpeed;       // Rotation speed (degrees per second)

public:
    // CONSTRUCTOR
    Obstacle(sf::Vector2f startPos, sf::Color col, float speed);

    // VIRTUAL DESTRUCTOR (Important for polymorphism!)
    virtual ~Obstacle() {}

    // VIRTUAL METHODS (Can be overridden by child classes)
    virtual void update(float dt);
    virtual void draw(sf::RenderWindow& window);
    virtual bool isColorWall() const { return false; }

    // GETTERS
    sf::Vector2f getPosition() const { return position; }
    sf::FloatRect getBounds() const { return shape.getGlobalBounds(); }
    bool active() const { return isActive; }
    sf::Color getColor() const { return color; }

    // SETTERS
    void deactivate() { isActive = false; }
};

#endif
```

**Teacher Question:** "Why virtual destructor?"
**Answer:** "When we delete a pointer to Obstacle that actually points to a ColorWallObstacle:

```cpp
Obstacle* obs = new ColorWallObstacle(...);
delete obs;  // Which destructor is called?
```

Without virtual destructor: Only Obstacle::~Obstacle() is called → MEMORY LEAK!
With virtual destructor: ColorWallObstacle::~ColorWallObstacle() is called first, then Obstacle::~Obstacle() → Proper cleanup!"

---

## FILE 5: Obstacle.cpp

**Purpose:** Obstacle class implementation
**Location:** `src/Obstacle.cpp`
**Lines of Code:** 39

### Complete Code with Explanations:

```cpp
#include "Obstacle.h"

Obstacle::Obstacle(sf::Vector2f startPos, sf::Color col, float speed) {
    // INITIALIZE PROPERTIES
    position = startPos;
    color = col;
    isActive = true;  // Newly created obstacle is active
    rotation = 0;     // Start at 0 degrees
    rotationSpeed = 180.0f;  // Rotate 180 degrees per second (half rotation)

    // CREATE SHAPE
    shape.setSize(sf::Vector2f(OBSTACLE_WIDTH, OBSTACLE_HEIGHT));  // 40x40 square
    shape.setOrigin(OBSTACLE_WIDTH / 2, OBSTACLE_HEIGHT / 2);      // Center origin
    shape.setFillColor(color);

    // STYLING
    shape.setOutlineThickness(2.0f);
    shape.setOutlineColor(sf::Color::White);

    // SET VELOCITY
    // Negative speed because obstacles move LEFT (negative X direction)
    velocity = sf::Vector2f(-speed, 0);
}

void Obstacle::update(float dt) {
    if (!isActive) return;  // Don't update inactive obstacles

    // MOVE LEFT
    position += velocity * dt;

    // ROTATE
    rotation += rotationSpeed * dt;

    // UPDATE VISUAL
    shape.setPosition(position);
    shape.setRotation(rotation);

    // DEACTIVATE IF OFF-SCREEN
    // When obstacle has moved completely off left side
    if (position.x < -OBSTACLE_WIDTH) {
        isActive = false;  // Mark for removal
    }
}

void Obstacle::draw(sf::RenderWindow& window) {
    if (isActive) {
        window.draw(shape);
    }
}
```

**Teacher Question:** "Why check isActive before drawing?"
**Answer:** "Performance! Obstacles that have moved off-screen are marked inactive but not immediately deleted (for efficiency). We don't want to waste time drawing invisible obstacles."

---

## FILE 6: ColorWallObstacle.h

**Purpose:** Special obstacle that requires color matching
**Location:** `include/ColorWallObstacle.h`
**Type:** Header file (child class)
**Lines of Code:** 28

### Complete Code with Explanations:

```cpp
#ifndef COLORWALLOBSTACLE_H
#define COLORWALLOBSTACLE_H

#include "Obstacle.h"
#include "Config.h"

// INHERITANCE DECLARATION
// "public Obstacle" means ColorWallObstacle IS-A Obstacle
// All public/protected members of Obstacle are accessible
class ColorWallObstacle : public Obstacle {
private:
    // ADDITIONAL PROPERTIES (not in base class)
    sf::Color requiredColor;  // Color player must match
    bool isTall;              // Flag indicating this is a tall wall

public:
    // CONSTRUCTOR
    // Takes same parameters as Obstacle
    ColorWallObstacle(sf::Vector2f startPos, sf::Color col, float speed);

    // OVERRIDE draw() to have different visual appearance
    // override keyword tells compiler we're replacing parent's version
    void draw(sf::RenderWindow& window) override;

    // GETTER
    sf::Color getRequiredColor() const { return requiredColor; }

    // OVERRIDE isColorWall() to return true
    bool isColorWall() const override { return true; }
};

#endif
```

**Teacher Question:** "What's the difference between this and Obstacle?"
**Answer:**
- **Inherited (same):** position, velocity, shape, update(), getPosition(), etc.
- **Different:**
  - Size: 3x wider, 0.8x screen height
  - Visual: Glow effect
  - Behavior: isColorWall() returns true
  - Rotation: Set to 0 (no spinning)"

---

## FILE 7: ColorWallObstacle.cpp

**Purpose:** ColorWallObstacle implementation
**Location:** `src/ColorWallObstacle.cpp`
**Lines of Code:** 34

### Complete Code with Explanations:

```cpp
#include "ColorWallObstacle.h"

ColorWallObstacle::ColorWallObstacle(sf::Vector2f startPos, sf::Color col, float speed)
    : Obstacle(startPos, col, speed) {  // CALL PARENT CONSTRUCTOR
    // This line calls Obstacle's constructor first!
    // After parent is constructed, we customize it...

    requiredColor = col;
    isTall = true;

    // RESIZE THE SHAPE (inherited from parent)
    // Make it MUCH taller to force color matching
    shape.setSize(sf::Vector2f(OBSTACLE_WIDTH * 3, WINDOW_HEIGHT * 0.8f));
    // Width: 40 * 3 = 120 pixels
    // Height: 720 * 0.8 = 576 pixels (80% of screen)

    shape.setOrigin(OBSTACLE_WIDTH * 1.5f, WINDOW_HEIGHT * 0.4f);
    // Center the origin for the new size

    shape.setFillColor(requiredColor);

    // DIFFERENT STYLING
    shape.setOutlineThickness(5.0f);  // Thicker than regular obstacles (2.0f)
    shape.setOutlineColor(sf::Color::White);

    // DISABLE ROTATION
    rotationSpeed = 0.0f;  // Color walls don't spin
}

void ColorWallObstacle::draw(sf::RenderWindow& window) {
    if (!active()) return;

    // DRAW GLOW EFFECT FIRST
    sf::RectangleShape glow = shape;  // Copy the shape

    // Make glow semi-transparent
    glow.setFillColor(sf::Color(requiredColor.r, requiredColor.g, requiredColor.b, 100));
    // RGB same as wall, alpha = 100 (semi-transparent)

    glow.setOutlineThickness(10.0f);  // Bigger outline
    glow.setOutlineColor(sf::Color(requiredColor.r, requiredColor.g, requiredColor.b, 50));
    // Even more transparent outline

    window.draw(glow);  // Draw glow first (back layer)

    // DRAW MAIN WALL
    window.draw(shape);  // Draw solid wall on top
}
```

**Teacher Question:** "How does constructor chaining work?"
**Answer:** "When we create a ColorWallObstacle:

1. **ColorWallObstacle constructor starts**
2. `: Obstacle(startPos, col, speed)` - **Parent constructor runs FIRST**
   - Sets up position, velocity, shape, rotation, etc.
3. **Back to ColorWallObstacle constructor**
   - Customizes the inherited shape (makes it taller)
   - Sets rotation speed to 0
   - Adds glow effect

This is called 'constructor initialization list' - efficient way to call parent constructor."

---

## FILE 8: Game.h

**Purpose:** Main game controller class
**Location:** `include/Game.h`
**Type:** Header file
**Lines of Code:** 82

### Complete Code with Explanations:

```cpp
#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include "Config.h"
#include "Player.h"
#include "Obstacle.h"
#include "ColorWallObstacle.h"
#include "PowerUp.h"
#include "ParticleSystem.h"
#include "UIManager.h"

// GAME STATE ENUM
// Determines what's currently happening in the game
enum class GameState {
    MENU,        // Showing main menu
    PLAYING,     // Active gameplay
    GAME_OVER    // Game over screen
};

class Game {
private:
    // ============================================
    // WINDOW & STATE
    // ============================================
    sf::RenderWindow window;  // The game window
    GameState state;          // Current game state

    // ============================================
    // GAME OBJECTS (COMPOSITION)
    // ============================================
    Player player;  // THE player (singular)

    // POLYMORPHIC CONTAINERS
    // Can hold both Obstacle AND ColorWallObstacle!
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    std::vector<std::unique_ptr<PowerUp>> powerUps;

    ParticleSystem particles;  // Visual effects manager
    UIManager ui;              // UI rendering manager

    // ============================================
    // GAME STATISTICS
    // ============================================
    int score;                     // Player's score
    int combo;                     // Consecutive dodges
    float obstacleSpawnTimer;      // Time until next obstacle
    float currentObstacleSpeed;    // Current obstacle speed (increases)
    float currentSpawnTime;        // Current spawn rate (decreases)
    float powerUpSpawnTimer;       // Time until next power-up
    float colorWallSpawnTimer;     // Time until next color wall

    // ============================================
    // VISUAL EFFECTS
    // ============================================
    float shakeIntensity;      // How strong the shake is
    float shakeTimer;          // How long shake lasts
    sf::Vector2f cameraOffset; // Camera displacement for shake

    std::vector<sf::CircleShape> backgroundStars;  // Decorative stars

    // ============================================
    // SOUND EFFECTS
    // ============================================
    sf::SoundBuffer dashBuffer;      // Dash sound data
    sf::Sound dashSound;              // Dash sound player

    sf::SoundBuffer wallPassBuffer;  // Color wall pass sound data
    sf::Sound wallPassSound;          // Color wall pass sound player

    sf::Music backgroundMusic;        // Background music (streams from file)

public:
    // CONSTRUCTOR
    Game();

    // MAIN GAME LOOP
    void run();  // Runs until window is closed

private:
    // ============================================
    // GAME LOOP COMPONENTS
    // ============================================
    void processEvents();  // Handle input
    void update(float dt); // Update game logic
    void render();         // Draw everything

    // ============================================
    // STATE MANAGEMENT
    // ============================================
    void startGame();   // Transition to PLAYING state
    void resetGame();   // Reset all game variables
    void gameOver();    // Transition to GAME_OVER state

    // ============================================
    // SPAWNING (FACTORY METHODS)
    // ============================================
    void spawnObstacle();   // Create regular obstacle
    void spawnPowerUp();    // Create power-up
    void spawnColorWall();  // Create color wall obstacle

    // ============================================
    // GAME LOGIC
    // ============================================
    void checkCollisions();   // Detect and handle all collisions
    void updateDifficulty();  // Scale difficulty based on score
    void screenShake(float intensity);  // Trigger screen shake effect

    // ============================================
    // HELPER METHODS
    // ============================================
    sf::Color getRandomColor();  // Get random color from palette
    void createBackground();      // Generate background stars
};

#endif
```

**Teacher Question:** "Why is Game the 'controller'?"
**Answer:** "Game follows the Model-View-Controller (MVC) pattern:
- **Model:** Player, Obstacle, PowerUp (data & logic)
- **View:** UIManager, ParticleSystem (presentation)
- **Controller:** Game (coordinates everything)

Game doesn't know HOW to draw a player or HOW particles work - it just tells them WHEN to do their job."

---

## FILE 9: Game.cpp (MOST IMPORTANT FILE)

**Purpose:** Game class implementation - the heart of the program
**Location:** `src/Game.cpp`
**Lines of Code:** 350+

### Constructor - Detailed Breakdown:

```cpp
Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE) {
    // CREATE WINDOW
    // sf::VideoMode(1280, 720) creates window size
    // WINDOW_TITLE = "Color Swap Runner(Upgraded)"

    // SET FRAME RATE LIMIT
    window.setFramerateLimit(FPS);  // 60 FPS
    // Why? Prevents CPU from running at 100%, saves battery, consistent gameplay

    // SET INITIAL STATE
    state = GameState::MENU;  // Start at menu

    // INITIALIZE GAME VARIABLES
    score = 0;
    combo = 0;
    obstacleSpawnTimer = 0;
    powerUpSpawnTimer = 0;
    colorWallSpawnTimer = 0;
    currentObstacleSpeed = OBSTACLE_SPEED;    // Start at 300
    currentSpawnTime = OBSTACLE_SPAWN_TIME;   // Start at 1.5 seconds
    shakeIntensity = 0;
    shakeTimer = 0;

    // LOAD SOUNDS
    if (dashBuffer.loadFromFile("assets/sounds/Dash.wav")) {
        dashSound.setBuffer(dashBuffer);
        dashSound.setVolume(70);  // 70% volume
    }

    if (wallPassBuffer.loadFromFile("assets/sounds/Bababooey.wav")) {
        wallPassSound.setBuffer(wallPassBuffer);
        wallPassSound.setVolume(80);  // 80% volume
    }

    // LOAD BACKGROUND MUSIC
    if (backgroundMusic.openFromFile("assets/sounds/bg_sound.wav")) {
        backgroundMusic.setLoop(true);   // Loop forever
        backgroundMusic.setVolume(30);   // Quieter (30%)
        backgroundMusic.play();          // Start immediately
    }

    // CREATE BACKGROUND
    createBackground();  // Generate 100 random stars
}
```

### run() - The Main Game Loop:

```cpp
void Game::run() {
    sf::Clock clock;  // Tracks elapsed time

    // GAME LOOP
    // Runs continuously until window is closed
    while (window.isOpen()) {
        // CALCULATE DELTA TIME
        float dt = clock.restart().asSeconds();
        // dt = time since last frame (usually ~0.0167 seconds at 60 FPS)

        // THREE PHASES OF GAME LOOP
        processEvents();  // Phase 1: Input
        update(dt);       // Phase 2: Logic
        render();         // Phase 3: Drawing

        // Window automatically displays after render()
    }
}
```

**Teacher Question:** "Why three separate methods?"
**Answer:** "Separation of concerns:
- `processEvents()`: Reads keyboard/mouse, handles window events
- `update()`: Updates positions, checks collisions, manages game state
- `render()`: Only draws to screen

Benefits:
1. Easier to debug (problem with rendering? Only check render())
2. Easier to test (can test update() without rendering)
3. Can swap implementations (e.g., replace processEvents with AI)
4. Follows Single Responsibility Principle"

### processEvents() - Input Handling:

```cpp
void Game::processEvents() {
    sf::Event event;

    // POLL ALL EVENTS
    // Processes all keyboard, mouse, window events from queue
    while (window.pollEvent(event)) {

        // WINDOW CLOSED
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        // KEY PRESSED
        if (event.type == sf::Event::KeyPressed) {

            // ESCAPE = QUIT
            if (event.key.code == sf::Keyboard::Escape) {
                window.close();
            }

            // ENTER = START/RESTART
            if (event.key.code == sf::Keyboard::Enter) {
                if (state == GameState::MENU) {
                    startGame();  // Menu → Playing
                } else if (state == GameState::GAME_OVER) {
                    resetGame();  // Clear everything
                    startGame();  // Game Over → Playing
                }
            }

            // SPACE = DASH (only while playing)
            if (event.key.code == sf::Keyboard::Space && state == GameState::PLAYING) {
                if (player.canDash()) {
                    player.dash();
                    particles.emit(player.getPosition(), COLOR_BLUE, 20);
                    dashSound.play();  // Play dash sound
                }
            }

            // C = CHANGE COLOR (only while playing)
            if (event.key.code == sf::Keyboard::C && state == GameState::PLAYING) {
                player.changeColor();
                particles.emit(player.getPosition(), player.getColor(), 15);
            }
        }
    }
}
```

**Teacher Question:** "What's the difference between Event and Keyboard::isKeyPressed()?"
**Answer:**
- `Event`: Triggered ONCE when key is pressed/released (good for actions)
- `isKeyPressed()`: Returns true WHILE key is held (good for movement)

Example:
- Dash uses Event - one press = one dash
- Movement uses isKeyPressed() - hold W = continuous movement"

### update() - Game Logic (CRITICAL METHOD):

```cpp
void Game::update(float dt) {
    // ONLY UPDATE IF PLAYING
    if (state != GameState::PLAYING) return;

    // ========================================
    // UPDATE PLAYER
    // ========================================
    player.update(dt);  // Move, dash, timers
    player.drawTrail(window, particles);  // Emit trail particles

    // ========================================
    // SPAWN & UPDATE OBSTACLES
    // ========================================
    obstacleSpawnTimer += dt;
    if (obstacleSpawnTimer >= currentSpawnTime) {
        spawnObstacle();      // Create new obstacle
        obstacleSpawnTimer = 0;  // Reset timer
    }

    // Update all existing obstacles
    for (auto& obstacle : obstacles) {
        obstacle->update(dt);  // Move left, rotate
    }

    // ========================================
    // SPAWN & UPDATE POWER-UPS
    // ========================================
    powerUpSpawnTimer += dt;
    if (powerUpSpawnTimer >= 5.0f) {  // Every 5 seconds
        spawnPowerUp();
        powerUpSpawnTimer = 0;
    }

    for (auto& powerUp : powerUps) {
        powerUp->update(dt);
    }

    // ========================================
    // SPAWN & UPDATE COLOR WALLS
    // ========================================
    colorWallSpawnTimer += dt;
    if (colorWallSpawnTimer >= COLOR_WALL_SPAWN_TIME) {  // Every 8 seconds
        spawnColorWall();
        colorWallSpawnTimer = 0;
    }

    // ========================================
    // UPDATE PARTICLES
    // ========================================
    particles.update(dt);  // Move particles, fade out, remove dead ones

    // ========================================
    // CHECK COLLISIONS
    // ========================================
    checkCollisions();  // Player vs obstacles, player vs power-ups

    // ========================================
    // UPDATE UI
    // ========================================
    ui.updateScore(score);
    ui.updateCombo(combo);
    ui.updateDashCooldown(player.getDashCooldown());

    // ========================================
    // UPDATE DIFFICULTY
    // ========================================
    updateDifficulty();  // Make game harder as score increases

    // ========================================
    // UPDATE SCREEN SHAKE
    // ========================================
    if (shakeTimer > 0) {
        shakeTimer -= dt;
        // Random offset for shake effect
        cameraOffset.x = (rand() % 100 - 50) / 50.0f * shakeIntensity;
        cameraOffset.y = (rand() % 100 - 50) / 50.0f * shakeIntensity;
    } else {
        cameraOffset = sf::Vector2f(0, 0);  // No shake
    }

    // ========================================
    // CLEANUP INACTIVE OBJECTS
    // ========================================
    // Remove obstacles that have moved off-screen
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [](const std::unique_ptr<Obstacle>& obs) {
                return !obs->active();
            }),
        obstacles.end()
    );

    // Remove collected power-ups
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(),
            [](const std::unique_ptr<PowerUp>& pw) {
                return !pw->active();
            }),
        powerUps.end()
    );
}
```

**Teacher Question:** "What's that lambda expression in erase?"
**Answer:** "It's an anonymous function:

```cpp
[](const std::unique_ptr<Obstacle>& obs) { return !obs->active(); }
// [ ] = capture clause (empty = capture nothing)
// (...) = parameters
// { } = function body

// Equivalent to:
bool isInactive(const std::unique_ptr<Obstacle>& obs) {
    return !obs->active();
}
```

`remove_if` calls this function for each obstacle. If it returns true, that obstacle is marked for removal."

### checkCollisions() - Collision Detection (CRITICAL):

```cpp
void Game::checkCollisions() {
    sf::FloatRect playerBounds = player.getBounds();
    // FloatRect = {left, top, width, height} bounding box

    // ========================================
    // CHECK OBSTACLE COLLISIONS
    // ========================================
    for (const auto& obstacle : obstacles) {
        if (obstacle->active() && obstacle->getBounds().intersects(playerBounds)) {
            // COLLISION DETECTED!

            // CHECK IF IT'S A COLOR WALL
            if (obstacle->isColorWall()) {
                // COLOR WALL COLLISION

                // CHECK COLOR MATCH
                if (player.getColor() != obstacle->getColor()) {
                    // WRONG COLOR = GAME OVER!
                    gameOver();
                    return;
                }
                // Colors match = pass through safely
                // (Do nothing, continue loop)

            } else {
                // REGULAR OBSTACLE = ALWAYS GAME OVER
                gameOver();
                return;
            }
        }

        // ========================================
        // SCORING FOR PASSING OBSTACLES
        // ========================================
        // Check if obstacle has passed player
        if (obstacle->active() &&
            obstacle->getPosition().x + OBSTACLE_WIDTH/2 < player.getPosition().x &&
            obstacle->getPosition().x + OBSTACLE_WIDTH/2 > player.getPosition().x - 10) {
            // Obstacle just passed player!

            if (obstacle->isColorWall()) {
                // PASSED COLOR WALL
                score += SCORE_COLOR_WALL_PASS;  // +50 points
                particles.emit(obstacle->getPosition(), obstacle->getColor(), 30);
                wallPassSound.play();  // "BABABOOEY!"
            } else {
                // PASSED REGULAR OBSTACLE
                score += SCORE_PER_DODGE;  // +10 points
                particles.emit(obstacle->getPosition(), obstacle->getColor(), 15);
            }
            combo++;  // Increment combo counter
        }
    }

    // ========================================
    // CHECK POWER-UP COLLISIONS
    // ========================================
    for (auto& powerUp : powerUps) {
        if (powerUp->active() && powerUp->getBounds().intersects(playerBounds)) {
            // POWER-UP COLLECTED!
            score += SCORE_POWERUP;  // +50 points
            particles.emit(powerUp->getPosition(), COLOR_YELLOW, 25);
            powerUp->deactivate();  // Mark for removal
        }
    }
}
```

**Teacher Question:** "How does intersects() work?"
**Answer:** "AABB (Axis-Aligned Bounding Box) collision:

```
Rectangle A: {x1, y1, w1, h1}
Rectangle B: {x2, y2, w2, h2}

Collision if ALL these are true:
- A.left < B.right
- A.right > B.left
- A.top < B.bottom
- A.bottom > B.top

SFML's intersects() does this check for us automatically."
```

### spawnColorWall() - Factory Method:

```cpp
void Game::spawnColorWall() {
    // SPAWN POSITION
    // Center vertically, off-screen to the right
    sf::Vector2f pos(WINDOW_WIDTH + OBSTACLE_WIDTH * 2, WINDOW_HEIGHT / 2.0f);
    // Why OBSTACLE_WIDTH * 2? Color wall is 3x wider, needs more space to spawn off-screen

    // RANDOM COLOR
    sf::Color wallColor = getRandomColor();

    // CREATE COLOR WALL USING POLYMORPHISM
    // Even though we're creating ColorWallObstacle, we store it in Obstacle vector!
    obstacles.push_back(
        std::make_unique<ColorWallObstacle>(pos, wallColor, currentObstacleSpeed * 0.7f)
    );
    // 0.7f = 70% speed of regular obstacles (moves slower, easier to react to)
}
```

### updateDifficulty() - Progressive Difficulty:

```cpp
void Game::updateDifficulty() {
    // INCREASE DIFFICULTY EVERY 100 POINTS
    static int lastDifficultyScore = 0;  // Persistent across calls

    if (score > 0 && score >= lastDifficultyScore + 100) {
        // MAKE OBSTACLES SPAWN FASTER
        currentSpawnTime *= SPEED_INCREASE_RATE;  // 0.95f = 5% faster

        // MAKE OBSTACLES MOVE FASTER
        currentObstacleSpeed *= 1.05f;  // 5% faster

        // APPLY CAPS (prevent getting impossible)
        if (currentSpawnTime < 0.5f)
            currentSpawnTime = 0.5f;  // Minimum 0.5 seconds between obstacles
        if (currentObstacleSpeed > 600.0f)
            currentObstacleSpeed = 600.0f;  // Maximum speed

        lastDifficultyScore = score;  // Remember this threshold
    }

    // RESET COMBO IF TOO SLOW
    static float comboTimer = 0;
    comboTimer += 0.016f;  // Approximate dt
    if (comboTimer > 3.0f) {  // 3 seconds without dodging
        combo = 0;
        comboTimer = 0;
    }
}
```

**Teacher Question:** "Why use static variables?"
**Answer:** "static means the variable persists between function calls:

```cpp
void example() {
    int normal = 0;     // Reset to 0 every call
    static int persist = 0;  // Keeps value between calls

    normal++;
    persist++;
}

example();  // normal=1, persist=1
example();  // normal=1, persist=2
example();  // normal=1, persist=3
```

We use it to remember the last score threshold without making it a class member."

---

## FILES 10-14: Remaining Classes

I'll provide brief overviews of the remaining files:

### PowerUp.h/cpp
- Represents collectible power-ups (shield, slow time, score boost)
- Similar structure to Obstacle
- Pulsing animation using sine wave
- Glow effect using additive blending

### ParticleSystem.h/cpp
- Manages hundreds of small particles for visual effects
- Three emission types: trail, explosion, burst
- Lifetime-based cleanup
- Alpha fading for smooth disappearance

### UIManager.h/cpp
- Handles all text rendering
- Three screens: menu, gameplay HUD, game over
- Font loading with fallback
- Dynamic text updating (score, combo, dash cooldown)

### main.cpp
```cpp
#include "Game.h"

int main() {
    Game game;  // Create game object
    game.run(); // Run game loop
    return 0;   // Program ends when window closes
}
```
Simple entry point - all logic is in Game class.

---

# 4. GAME MECHANICS & LOGIC

## 4.1 Color Matching System

### How It Works:

1. **Player has 6 colors available:**
   - Red, Blue, Yellow, Green, Purple, Orange

2. **Press C to cycle colors:**
   ```
   Red → Blue → Yellow → Green → Purple → Orange → Red → ...
   ```

3. **Color walls spawn with random color**

4. **When collision occurs:**
   ```cpp
   if (playerColor == wallColor) {
       // PASS THROUGH SAFELY
       score += 50;
       play("Bababooey!");
   } else {
       // GAME OVER
       gameOver();
   }
   ```

### Strategic Depth:

- Must watch upcoming color wall
- Quick reaction needed (press C multiple times)
- Adds skill element beyond just dodging

## 4.2 Dash Mechanic

### Properties:
- **Duration:** 0.2 seconds
- **Speed:** 800 pixels/second (vs 350 normal)
- **Cooldown:** 1.0 second
- **Direction:** Uses current movement direction

### Use Cases:
1. Emergency dodge when surrounded
2. Quick repositioning
3. Speed through tight gaps
4. Reach power-ups quickly

### Implementation Detail:
```cpp
// During dash
position += dashDirection * DASH_SPEED * dt;
// Movement is overridden - input doesn't affect dash

// After dash ends
position += velocity * dt;
// Normal input-based movement resumes
```

## 4.3 Difficulty Progression

### Scaling Formula:

**Every 100 points:**
- Spawn time: `currentTime *= 0.95` (5% faster)
- Obstacle speed: `currentSpeed *= 1.05` (5% faster)

### Example Progression:

| Score | Spawn Time | Speed | Difficulty |
|-------|-----------|-------|------------|
| 0 | 1.50s | 300 px/s | Easy |
| 100 | 1.43s | 315 px/s | Easy+ |
| 200 | 1.35s | 331 px/s | Medium- |
| 300 | 1.28s | 347 px/s | Medium |
| 500 | 1.13s | 382 px/s | Medium+ |
| 1000 | 0.89s | 470 px/s | Hard |
| 1500 | 0.70s | 579 px/s | Hard+ |
| 2000 | 0.50s | 600 px/s | Maximum |

### Caps:
- **Min spawn time:** 0.5 seconds (can't get faster)
- **Max speed:** 600 px/s (speed plateau)

## 4.4 Scoring System

### Point Values:

| Action | Points |
|--------|--------|
| Dodge regular obstacle | +10 |
| Pass through color wall | +50 |
| Collect power-up | +50 |

### Combo System:
- Increments with each dodge
- Displayed when combo ≥ 5
- Resets after 3 seconds of no dodges
- No multiplier (future enhancement)

## 4.5 Collision Detection

### Algorithm: AABB (Axis-Aligned Bounding Box)

**Why AABB?**
- Fast computation (just rectangle comparisons)
- Good enough for square/rectangular shapes
- Built into SFML (intersects() method)

**Limitation:**
- Not pixel-perfect (uses rectangular bounds)
- Fine for this game style

**Alternative (not used):**
- Pixel-perfect collision (slower, unnecessary)
- Circle collision (doesn't match square shapes)

## 4.6 Screen Shake Effect

### How It Works:

```cpp
void Game::screenShake(float intensity) {
    shakeIntensity = intensity;  // e.g., 20.0f
    shakeTimer = 0.3f;            // Lasts 0.3 seconds
}

// In update():
cameraOffset.x = (rand() % 100 - 50) / 50.0f * shakeIntensity;
cameraOffset.y = (rand() % 100 - 50) / 50.0f * shakeIntensity;
// Random offset: -intensity to +intensity

// In render():
view.setCenter(WINDOW_WIDTH/2 + cameraOffset.x,
               WINDOW_HEIGHT/2 + cameraOffset.y);
```

**Result:** Camera jitters randomly for visual impact on game over

---

# 5. CODE FLOW & EXECUTION

## 5.1 Program Startup Flow

```
1. main() starts
   ↓
2. Game game; (constructor runs)
   ├─ Create window
   ├─ Load sounds
   ├─ Start background music
   └─ Create background stars
   ↓
3. game.run() (main loop starts)
   ↓
4. MENU state (waiting for Enter)
```

## 5.2 Frame-by-Frame Execution (60 FPS)

### Single Frame Timeline:

```
Frame N (16.67ms at 60 FPS):
├─ 0ms: clock.restart() → get dt
├─ 1ms: processEvents()
│  ├─ Check keyboard
│  ├─ Check window events
│  └─ Trigger actions (dash, color change, etc.)
├─ 5ms: update(dt)
│  ├─ Update player position
│  ├─ Update all obstacles
│  ├─ Update particles
│  ├─ Check collisions
│  ├─ Update timers
│  └─ Remove inactive objects
├─ 10ms: render()
│  ├─ Clear screen
│  ├─ Draw background
│  ├─ Draw obstacles
│  ├─ Draw player
│  ├─ Draw particles
│  ├─ Draw UI
│  └─ Display window
└─ 16.67ms: Frame complete, repeat
```

## 5.3 Object Lifecycle

### Obstacle Lifecycle:

```
1. SPAWN
   spawnObstacle() → make_unique<Obstacle>()
   ↓
2. ACTIVE
   - update() moves it left
   - draw() renders it
   - checkCollisions() checks player collision
   ↓
3. SCORING
   - Player passes it → score increases
   ↓
4. OFF-SCREEN
   - position.x < -OBSTACLE_WIDTH
   - isActive = false
   ↓
5. REMOVAL
   - remove_if() in update()
   - unique_ptr automatically deletes object
```

### ColorWallObstacle Lifecycle (similar but with color check):

```
1. SPAWN
   spawnColorWall() → make_unique<ColorWallObstacle>()
   ↓
2. APPROACH
   - Player sees wall color
   - Player presses C to match color
   ↓
3. COLLISION CHECK
   if (playerColor == wallColor) {
       PASS THROUGH
   } else {
       GAME OVER
   }
   ↓
4. SCORING
   - Player passes → +50 points, "Bababooey" sound
   ↓
5. OFF-SCREEN & REMOVAL
```

## 5.4 State Transitions

```
MENU State:
- Display: Title, instructions
- Input: Enter key
- Action: startGame()
- Transition: → PLAYING

PLAYING State:
- Display: Game objects, score, UI
- Input: WASD, Space, C
- Action: Game logic runs
- Transition: Collision → GAME_OVER

GAME_OVER State:
- Display: Final score, restart prompt
- Input: Enter key
- Action: resetGame() + startGame()
- Transition: → PLAYING
```

---

# 6. CLASS RELATIONSHIPS & ARCHITECTURE

## 6.1 Class Diagram

```
                    ┌──────────────┐
                    │   Config.h   │
                    │  (Constants) │
                    └──────┬───────┘
                           │ [uses]
              ┌────────────┼────────────┐
              │            │            │
              ▼            ▼            ▼
       ┌──────────┐  ┌─────────┐  ┌──────────┐
       │ Player   │  │Obstacle │  │PowerUp   │
       └──────────┘  └────┬────┘  └──────────┘
                          │
                          │ [inherits]
                          │
                   ┌──────▼──────────┐
                   │ColorWallObstacle│
                   └─────────────────┘

              ┌───────────────────────┐
              │        Game           │
              │   (Controller)        │
              ├───────────────────────┤
              │ HAS-A:                │
              │  - Player             │
              │  - vector<Obstacle*>  │
              │  - vector<PowerUp*>   │
              │  - ParticleSystem     │
              │  - UIManager          │
              │  - Sounds             │
              └───────────────────────┘
```

## 6.2 Dependency Graph

```
main.cpp
  └─> Game
      ├─> Player
      │   └─> ParticleSystem
      ├─> Obstacle
      │   └─> ColorWallObstacle
      ├─> PowerUp
      ├─> ParticleSystem
      ├─> UIManager
      └─> Config.h (all files use this)
```

## 6.3 Memory Ownership

```
Stack (Automatic):
- Game object (in main)
- Player object (in Game)
- ParticleSystem (in Game)
- UIManager (in Game)

Heap (Dynamic):
- Obstacles (managed by unique_ptr)
- PowerUps (managed by unique_ptr)
- Particles (in vector inside ParticleSystem)

Ownership Rules:
- Game OWNS all objects
- When Game is destroyed, everything is automatically cleaned up
- No manual delete needed!
```

---

# 7. COMMON QUESTIONS & ANSWERS

## 7.1 OOP Questions

### Q: "Explain inheritance in your project"
**A:** "We have a base class `Obstacle` and a derived class `ColorWallObstacle`. ColorWallObstacle inherits all the properties and methods from Obstacle, such as position, velocity, and update(). This demonstrates code reuse - we don't have to rewrite all the obstacle behavior for color walls. We only write what's DIFFERENT (larger size, no rotation, color matching). This follows the DRY principle (Don't Repeat Yourself)."

### Q: "What is polymorphism and where do you use it?"
**A:** "Polymorphism means 'many forms'. In our project, we store both `Obstacle` and `ColorWallObstacle` objects in the same vector:

```cpp
std::vector<std::unique_ptr<Obstacle>> obstacles;
```

Even though the vector is typed as Obstacle, it can hold ColorWallObstacle too because ColorWallObstacle IS-A Obstacle. When we call methods like draw() or isColorWall(), the correct version is called based on the actual object type, not the pointer type. This is runtime polymorphism using virtual functions."

### Q: "Why use protected instead of private?"
**A:** "We changed obstacle member variables from private to protected when we added inheritance. Protected means:
- Other classes can't access it (like private)
- Child classes CAN access it (unlike private)

ColorWallObstacle needs to access the `shape` variable to resize it. If it was private, the child class couldn't access it and we'd have to use getter/setter methods for everything, which would be cumbersome."

### Q: "What is encapsulation?"
**A:** "Encapsulation means bundling data and methods together while hiding internal implementation. For example, in the Player class:

```cpp
private:
    sf::Vector2f position;  // Hidden!

public:
    sf::Vector2f getPosition() const;  // Controlled access
```

Outside code can't directly change position. It must use provided methods. This prevents invalid states (like position outside screen bounds) and makes the code more maintainable."

### Q: "Explain the virtual keyword"
**A:** "Virtual means 'this method can be overridden by child classes and should use dynamic dispatch'. Without virtual:

```cpp
Obstacle* obs = new ColorWallObstacle();
obs->draw();  // Calls Obstacle::draw() (WRONG!)
```

With virtual:
```cpp
Obstacle* obs = new ColorWallObstacle();
obs->draw();  // Calls ColorWallObstacle::draw() (CORRECT!)
```

The compiler creates a vtable (virtual function table) to look up the correct method at runtime."

## 7.2 Design Questions

### Q: "Why separate header and implementation files?"
**A:** "Three reasons:
1. **Organization:** Declarations in .h, definitions in .cpp
2. **Compilation Speed:** Changing .cpp doesn't require recompiling files that include the .h
3. **Hiding Implementation:** Users of the class only need to see the interface (.h), not the implementation details"

### Q: "Why use smart pointers instead of raw pointers?"
**A:** "Memory safety! With raw pointers:

```cpp
Obstacle* obs = new Obstacle();
// If we forget 'delete obs', MEMORY LEAK!
// If we 'delete obs' twice, CRASH!
```

With smart pointers:
```cpp
unique_ptr<Obstacle> obs = make_unique<Obstacle>();
// Automatically deleted when obs goes out of scope
// Can't accidentally delete twice
// Exception-safe
```

unique_ptr also makes ownership clear - there's exactly ONE owner."

### Q: "What design patterns did you use?"
**A:**
1. **Game Loop Pattern:** processEvents → update → render
2. **State Pattern:** GameState enum controls behavior
3. **Factory Pattern:** spawnObstacle(), spawnColorWall()
4. **Composition:** Game HAS-A Player, not IS-A Player
5. **MVC Pattern:** Model (game objects), View (rendering), Controller (Game class)"

### Q: "Why Config.h?"
**A:** "Single Source of Truth principle. All game constants in one place means:
- Easy balancing (change PLAYER_SPEED once)
- No magic numbers scattered in code
- Clear documentation of game parameters
- Easy for designers to tweak without touching code"

## 7.3 Technical Questions

### Q: "What is delta time and why use it?"
**A:** "Delta time (dt) is the time elapsed since the last frame. We use it for frame-rate independence:

```cpp
// Without dt:
position += velocity;  // Different speeds on different computers!

// With dt:
position += velocity * dt;  // Same speed everywhere
```

If game runs at 60 FPS, dt ≈ 0.0167 seconds
If game runs at 30 FPS, dt ≈ 0.0333 seconds

The movement per second is the same regardless of frame rate."

### Q: "How do you prevent memory leaks?"
**A:** "We use modern C++ memory management:
1. **std::unique_ptr:** Automatic deletion when pointer goes out of scope
2. **RAII:** Resources acquired in constructor, released in destructor
3. **No manual new/delete:** All dynamic allocation uses smart pointers
4. **STL containers:** vector, string automatically manage their memory

Result: Zero memory leaks!"

### Q: "Explain your collision detection"
**A:** "We use AABB (Axis-Aligned Bounding Box) collision:

```cpp
sf::FloatRect playerBounds = player.getBounds();
sf::FloatRect obstacleBounds = obstacle->getBounds();

if (playerBounds.intersects(obstacleBounds)) {
    // COLLISION!
}
```

intersects() checks if two rectangles overlap using simple comparisons:
- Is A's left edge left of B's right edge?
- Is A's right edge right of B's left edge?
- Is A's top edge above B's bottom edge?
- Is A's bottom edge below B's top edge?

If all are true, they're colliding. This is O(1) - very fast!"

### Q: "How does the color matching work technically?"
**A:** "When a collision is detected with a color wall:

```cpp
if (obstacle->isColorWall()) {
    // Compare sf::Color objects
    if (player.getColor() != obstacle->getColor()) {
        gameOver();  // Wrong color
    }
    // else: Colors match, pass through
}
```

sf::Color has overloaded operator!= that compares RGB values:
```cpp
sf::Color(255, 0, 100) != sf::Color(0, 255, 0)  // true
sf::Color(255, 0, 100) != sf::Color(255, 0, 100)  // false
```

Perfect match required - if even one RGB component differs, it's wrong!"

## 7.4 Gameplay Questions

### Q: "How does difficulty increase?"
**A:** "Every 100 points:
- Spawn time decreases by 5% (obstacles come faster)
- Obstacle speed increases by 5% (obstacles move faster)

Formula:
```cpp
currentSpawnTime *= 0.95;
currentObstacleSpeed *= 1.05;
```

But we have caps:
- Min spawn time: 0.5 seconds
- Max speed: 600 pixels/second

This creates a smooth difficulty curve that eventually plateaus."

### Q: "What is the game loop and why is it important?"
**A:** "The game loop is the core of any game:

```cpp
while (game is running) {
    1. Process Input (keyboard, mouse)
    2. Update Game State (physics, AI, collisions)
    3. Render Graphics (draw everything)
}
```

This ensures:
- Consistent execution order
- All updates happen before drawing
- Input is processed immediately
- Smooth animation (60 times per second)"

### Q: "How do you handle different frame rates?"
**A:** "Delta time! Every frame we calculate:

```cpp
float dt = clock.restart().asSeconds();
```

Then use it in all time-based calculations:
```cpp
position += velocity * dt;
timer += dt;
rotation += rotationSpeed * dt;
```

If frame rate drops (dt increases), objects move more per frame to compensate. If frame rate increases (dt decreases), objects move less per frame. Net result: constant speed in real time."

---

# 8. TECHNICAL IMPLEMENTATION DETAILS

## 8.1 SFML Basics Used

### Graphics Module:
- `sf::RenderWindow`: Game window
- `sf::RectangleShape`: Squares (player, obstacles)
- `sf::CircleShape`: Circles (power-ups, particles, stars)
- `sf::Color`: RGBA colors
- `sf::Vector2f`: 2D positions/velocities
- `sf::FloatRect`: Bounding boxes
- `sf::Text`: UI text rendering
- `sf::Font`: TrueType font loading

### System Module:
- `sf::Clock`: Time tracking
- `sf::Event`: Input events

### Audio Module:
- `sf::SoundBuffer`: Sound data storage
- `sf::Sound`: Sound playback
- `sf::Music`: Music streaming

## 8.2 C++ Features Used

### C++11/14/17 Features:
- `auto` keyword for type inference
- Range-based for loops
- Lambda expressions
- `std::unique_ptr` smart pointers
- `std::vector` containers
- `enum class` for type-safe enums
- `nullptr` instead of NULL
- Move semantics (with std::move)

### Object-Oriented Features:
- Classes and objects
- Inheritance (public inheritance)
- Polymorphism (virtual functions)
- Encapsulation (access modifiers)
- Constructors and destructors
- Const correctness
- Method overriding

## 8.3 Algorithms Used

### Random Number Generation:
```cpp
std::random_device rd;           // Seed
std::mt19937 gen(rd());          // Mersenne Twister engine
std::uniform_real_distribution<float> dist(min, max);
float random = dist(gen);
```

### Vector Normalization:
```cpp
float length = sqrt(x*x + y*y);
x = x / length * desiredSpeed;
y = y / length * desiredSpeed;
```

### Remove-Erase Idiom:
```cpp
obstacles.erase(
    std::remove_if(obstacles.begin(), obstacles.end(), predicate),
    obstacles.end()
);
```

## 8.4 Performance Considerations

### Optimizations Used:
1. **Object Pooling (Partial):** Objects marked inactive before deletion
2. **Early Returns:** Exit functions early when possible
3. **Const References:** Pass large objects by const reference
4. **Reserve Vector Capacity:** Could pre-allocate vector size
5. **Inline Functions:** Small getters in header (compiler can inline)

### Potential Optimizations (Not Implemented):
1. Spatial partitioning for collision detection
2. Dirty flag for UI updates
3. Object pool recycling
4. Background rendering caching

## 8.5 Compilation Process

### Compilation Steps:
```bash
# 1. Preprocessing
g++ -E Player.cpp → Expands #includes, #defines

# 2. Compilation
g++ -c Player.cpp → Creates Player.o (object file)

# 3. Linking
g++ *.o -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -o game.exe
→ Combines object files + libraries → executable

# 4. Loading (at runtime)
Windows loads game.exe + DLLs (sfml-graphics-2.dll, etc.)
```

### Why Separate Compilation?
- Only recompile changed files
- Faster build times
- Easier to manage large projects

---

# APPENDIX: QUICK REFERENCE

## Common Teacher Questions - Quick Answers

**Q: Main OOP concepts?**
A: Encapsulation (private data), Inheritance (ColorWallObstacle extends Obstacle), Polymorphism (virtual functions), Abstraction (simple interfaces)

**Q: Design patterns?**
A: Game Loop, State Pattern, Factory Pattern, Composition, MVC

**Q: Memory management?**
A: std::unique_ptr for dynamic objects, automatic cleanup, no leaks

**Q: Why delta time?**
A: Frame-rate independence - same speed on all computers

**Q: How does color matching work?**
A: Compare sf::Color objects in collision detection, exact RGB match required

**Q: What is polymorphism in this project?**
A: Obstacles vector holds both Obstacle and ColorWallObstacle, virtual functions ensure correct methods are called

**Q: Why separate .h and .cpp?**
A: Organization, faster compilation, hide implementation

**Q: How does difficulty scale?**
A: Every 100 points: spawn 5% faster, move 5% faster, with caps

## Code Statistics

- **Total Files:** 16 (8 .h, 7 .cpp, 1 main.cpp)
- **Total Classes:** 8
- **Total Lines:** ~1200
- **OOP Concepts:** 6+ demonstrated
- **Design Patterns:** 5 used
- **SFML Modules:** 3 (Graphics, System, Audio)

## Project Strengths

1. ✅ Clean OOP design
2. ✅ Proper inheritance hierarchy
3. ✅ Polymorphism demonstration
4. ✅ Memory-safe (smart pointers)
5. ✅ Frame-rate independent
6. ✅ Well-commented code
7. ✅ Modular architecture
8. ✅ Professional practices

---

**END OF DOCUMENTATION**

This documentation covers every aspect of your project. Study each section and you'll be able to answer ANY question about OOP concepts, design decisions, or technical implementation! Good luck with your presentation! 🚀
