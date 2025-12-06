# Color Swap Runner

A fast-paced 2D endless runner game built with C++ and SFML. Dodge obstacles, match colors, and collect coins while the game gets progressively harder!

## Overview

Color Swap Runner is an arcade-style game where you control a player that can change colors to pass through matching color walls. The game features smooth particle effects, background music, sound effects, and increasing difficulty as you progress. Your goal is to dodge regular obstacles, match your color with color walls, collect power-ups and coins, and rack up the highest score possible!

## Gameplay Features

- **Color Matching Mechanic**: Change your player's color using the 'C' key to pass through color walls
- **Dash Ability**: Quick dash movement with spacebar to avoid obstacles
- **Multiple Obstacle Types**: Regular obstacles and special color walls that require color matching
- **Collectibles**: Coins and power-ups scattered throughout the game
- **Particle Effects**: Visual feedback for dashing, collecting items, and passing obstacles
- **Progressive Difficulty**: Game speed increases as your score gets higher
- **Score & Combo System**: Build combos by successfully dodging obstacles
- **Sound Effects**: Dash sounds, wall pass sounds, and background music

## Controls

- **Arrow Keys / WASD**: Move the player
- **Spacebar**: Dash (has cooldown)
- **C Key**: Change player color
- **Enter**: Start game / Restart after game over
- **Escape**: Close game

## File Structure

### `/include` - Header Files

- **Config.h** - Central configuration file with all game constants (window size, speeds, colors, spawn times, scoring values)
- **Game.h** - Main game class that manages the game loop, states (menu, playing, game over), and coordinates all game objects
- **Player.h** - Player class with movement, dashing, and color changing functionality
- **Obstacle.h** - Base obstacle class with basic properties (position, color, movement)
- **ColorWallObstacle.h** - Special wall obstacle that requires color matching to pass through
- **PowerUp.h** - Power-up collectibles (shield, slow time, score boost)
- **Coin.h** - Collectible coins that add to your score
- **ParticleSystem.h** - Handles all particle effects (trails, explosions, collection effects)
- **UIManager.h** - Manages all UI elements (score display, combo counter, menu, game over screen)

### `/src` - Implementation Files

- **main.cpp** - Entry point of the program, creates and runs the game
- **Game.cpp** - Implements game loop, collision detection, spawning logic, difficulty scaling, and game state management
- **Player.cpp** - Implements player movement, dash mechanics, color switching, and visual rendering
- **Obstacle.cpp** - Basic obstacle behavior and rendering
- **ColorWallObstacle.cpp** - Special wall obstacle implementation with full-height walls
- **PowerUp.cpp** - Power-up spawning and collection mechanics
- **Coin.cpp** - Coin spawning, rotation animation, and collection
- **ParticleSystem.cpp** - Particle generation, physics, and rendering
- **UIManager.cpp** - UI rendering for all game states

### `/assets`

- **fonts/** - Contains font files for text rendering
- **sounds/** - Contains all audio files (Dash.wav, Bababooey.wav for wall pass, bg_sound.wav for background music)

## How It Works

The game uses Object-Oriented Programming principles:

- **Inheritance**: ColorWallObstacle inherits from Obstacle base class
- **Polymorphism**: Virtual functions allow different obstacle types to behave differently
- **Encapsulation**: Each class manages its own data and behavior
- **Smart Pointers**: Uses `std::unique_ptr` for automatic memory management of game objects

The main game loop runs at 60 FPS and handles:
1. Event processing (keyboard input)
2. Update logic (moving objects, checking collisions, spawning)
3. Rendering (drawing all game objects and UI)

## Compilation & Running

### Prerequisites
- MinGW GCC compiler (we used 7.3.0)
- SFML 2.5.0 library

### Build Instructions

1. Navigate to the project directory:
```bash
cd "OOP_PROJECT - Final"
```

2. Compile the game using this command:
```bash
g++ -Iinclude -I"C:\Users\hassa\Desktop\SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.0\include" src/*.cpp -L"C:\Users\hassa\Desktop\SFML-2.5.0-windows-gcc-7.3.0-mingw-64-bit\SFML-2.5.0\lib" -o game.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

**Note**: You'll need to update the SFML paths to match your SFML installation location.

3. Run the game:
```bash
./game.exe
```

Make sure the SFML DLL files are in the same directory as game.exe, or add the SFML bin folder to your PATH.

## Game Mechanics

### Scoring
- Dodge regular obstacle: **10 points**
- Pass through color wall (with matching color): **50 points**
- Collect power-up: **50 points**
- Collect coin: **25 points**

### Difficulty Progression
Every 100 points:
- Obstacle spawn rate increases
- Obstacle speed increases (caps at 600 units/second)
- Spawn time decreases (minimum 0.5 seconds)

## OOP Concepts Used

- **Classes and Objects**: Game, Player, Obstacle, PowerUp, Coin, ParticleSystem, UIManager
- **Inheritance**: ColorWallObstacle extends Obstacle
- **Polymorphism**: Virtual functions for draw() and update()
- **Encapsulation**: Private member variables with public getters
- **Composition**: Game class contains Player, multiple Obstacles, PowerUps, etc.
- **Smart Pointers**: Modern C++ memory management with std::unique_ptr

## Credits

Built as an Object-Oriented Programming project using C++ and SFML.
