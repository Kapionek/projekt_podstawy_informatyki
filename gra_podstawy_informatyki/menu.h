#pragma once
#include <SFML/Graphics.hpp>
#include "obiekty.h"

class Menu {
public:


    Menu(sf::RenderWindow& window, std::vector<Object>& walls);

    void handleEvent(const sf::Event& event);
    void draw();
    bool shouldStartGame() const;
    bool shouldExit() const;

private:
    void updateSelection();

    sf::RenderWindow& window;
    std::vector<Object>& wallsRef;

    sf::Font font;

    sf::Text playText;
    sf::Text exitText;

    int selected;
    bool startGame;
    bool exitGame;
};
