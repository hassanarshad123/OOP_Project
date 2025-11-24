#ifndef UIMANAGER_H
#define UIMANAGER_H

#include <SFML/Graphics.hpp>
#include "Config.h"
#include <string>

class UIManager {
private:
    sf::Font font;
    sf::Text scoreText;
    sf::Text comboText;
    sf::Text dashCooldownText;
    sf::Text gameOverText;
    sf::Text instructionText;
    
    bool fontLoaded;
    
public:
    UIManager();
    
    // Load font
    bool loadFont(const std::string& path);
    
    // Update displays
    void updateScore(int score);
    void updateCombo(int combo);
    void updateDashCooldown(float cooldown);
    
    // Draw
    void drawGameUI(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window, int finalScore);
    void drawMenu(sf::RenderWindow& window);
};

#endif
