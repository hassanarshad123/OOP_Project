#include "UIManager.h"

UIManager::UIManager() : fontLoaded(false) {
    // Try to load font
    if (font.loadFromFile("assets/fonts/ARIALN.TTF")) {
        fontLoaded = true;
    } else if (font.loadFromFile("ARIALN.TTF")) {
        fontLoaded = true;
    }
    
    // Setup text objects
    scoreText.setFont(font);
    scoreText.setCharacterSize(36);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(20, 20);
    scoreText.setOutlineThickness(2);
    scoreText.setOutlineColor(sf::Color::Black);
    
    comboText.setFont(font);
    comboText.setCharacterSize(48);
    comboText.setFillColor(COLOR_YELLOW);
    comboText.setPosition(WINDOW_WIDTH / 2 - 100, 100);
    comboText.setOutlineThickness(3);
    comboText.setOutlineColor(sf::Color::Black);
    
    dashCooldownText.setFont(font);
    dashCooldownText.setCharacterSize(24);
    dashCooldownText.setFillColor(COLOR_GREEN);
    dashCooldownText.setPosition(20, WINDOW_HEIGHT - 50);
    
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(COLOR_RED);
    gameOverText.setString("GAME OVER");
    gameOverText.setOutlineThickness(4);
    gameOverText.setOutlineColor(sf::Color::Black);
    sf::FloatRect bounds = gameOverText.getLocalBounds();
    gameOverText.setOrigin(bounds.width / 2, bounds.height / 2);
    gameOverText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100);
    
    instructionText.setFont(font);
    instructionText.setCharacterSize(32);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setOutlineThickness(2);
    instructionText.setOutlineColor(sf::Color::Black);
}

bool UIManager::loadFont(const std::string& path) {
    if (font.loadFromFile(path)) {
        fontLoaded = true;
        return true;
    }
    return false;
}

void UIManager::updateScore(int score) {
    scoreText.setString("Score: " + std::to_string(score));
}

void UIManager::updateCombo(int combo) {
    if (combo >= COMBO_THRESHOLD) {
        comboText.setString("COMBO x" + std::to_string(combo));
    } else {
        comboText.setString("");
    }
}

void UIManager::updateDashCooldown(float cooldown) {
    if (cooldown > 0) {
        dashCooldownText.setString("Dash: " + 
            std::to_string(static_cast<int>(cooldown * 10) / 10.0f) + "s");
        dashCooldownText.setFillColor(COLOR_ORANGE);
    } else {
        dashCooldownText.setString("Dash: READY [SPACE]");
        dashCooldownText.setFillColor(COLOR_GREEN);
    }
}

void UIManager::drawGameUI(sf::RenderWindow& window) {
    if (!fontLoaded) return;
    
    window.draw(scoreText);
    window.draw(comboText);
    window.draw(dashCooldownText);
}

void UIManager::drawGameOver(sf::RenderWindow& window, int finalScore) {
    if (!fontLoaded) return;
    
    // Dark overlay
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 150));
    window.draw(overlay);
    
    // Game over text
    window.draw(gameOverText);
    
    // Final score
    sf::Text finalScoreText;
    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(48);
    finalScoreText.setFillColor(sf::Color::White);
    finalScoreText.setString("Final Score: " + std::to_string(finalScore));
    finalScoreText.setOutlineThickness(3);
    finalScoreText.setOutlineColor(sf::Color::Black);
    sf::FloatRect bounds = finalScoreText.getLocalBounds();
    finalScoreText.setOrigin(bounds.width / 2, bounds.height / 2);
    finalScoreText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    window.draw(finalScoreText);
    
    // Restart instruction
    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(32);
    restartText.setFillColor(COLOR_GREEN);
    restartText.setString("Press ENTER to restart");
    restartText.setOutlineThickness(2);
    restartText.setOutlineColor(sf::Color::Black);
    bounds = restartText.getLocalBounds();
    restartText.setOrigin(bounds.width / 2, bounds.height / 2);
    restartText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 100);
    window.draw(restartText);
}

void UIManager::drawMenu(sf::RenderWindow& window) {
    if (!fontLoaded) return;
    
    // Title
    sf::Text title;
    title.setFont(font);
    title.setCharacterSize(96);
    title.setFillColor(COLOR_BLUE);
    title.setString("COLOR SAWP RUNNER!(2.0)");
    title.setOutlineThickness(5);
    title.setOutlineColor(sf::Color::White);
    sf::FloatRect bounds = title.getLocalBounds();
    title.setOrigin(bounds.width / 2, bounds.height / 2);
    title.setPosition(WINDOW_WIDTH / 2, 150);
    window.draw(title);
    
    // Instructions
    instructionText.setString("WASD or Arrow Keys to Move\nSPACE to Dash\nDodge the obstacles!\n\nPress ENTER to Start");
    bounds = instructionText.getLocalBounds();
    instructionText.setOrigin(bounds.width / 2, bounds.height / 2);
    instructionText.setPosition(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50);
    window.draw(instructionText);
}
