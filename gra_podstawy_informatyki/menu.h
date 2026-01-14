#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "obiekty.h"

class Menu {
public:
    // Konstruktor i metody publiczne (dostêpne w main.cpp)
    Menu(sf::RenderWindow& window, std::vector<Object>& walls);

    void handleEvent(const sf::Event& event);
    void draw();
    bool shouldStartGame() const;
    bool shouldExit() const;
    void setEndScreen(int winner);
    void resetFlags();

private:
    // Metody i zmienne prywatne (u¿ywane tylko wewn¹trz Menu.cpp)
    void updateSelection();

    sf::RenderWindow& window;
    std::vector<Object>& wallsRef;
    sf::Font font;

    sf::Text playText;
    sf::Text exitText;
    sf::Text resultText; 

    int selected;
    bool startGame;
    bool exitGame;
    bool isGameOver = false; 
};