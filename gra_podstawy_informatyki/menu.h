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

    float getVolume() const { return volume; }
    std::string getNick1() const { return nick1; }
    std::string getNick2() const { return nick2; }

private:
    // Metody i zmienne prywatne (u¿ywane tylko wewn¹trz Menu.cpp)
    void updateSelection();
    void centerText(sf::Text& text, float y);

    sf::RenderWindow& window;
    std::vector<Object>& wallsRef;
    sf::Font font;

    sf::Text playText;
    sf::Text exitText;
    sf::Text volumeText; //  Wyœwietlanie g³oœnoœci
    sf::Text resultText; 

    // Elementy wpisywania nicków
    sf::Text promptText;   // Napis "Wpisz nick gracza"
    sf::Text inputDisplay; // Wyœwietla to, co aktualnie piszemy
    std::string nick1 = "Gracz 1"; 
    std::string nick2 = "Gracz 2"; 
    std::string currentInput;      // Bufor na wpisywany tekst

    int selected;
    float volume = 100.f;     //  Wartoœæ g³oœnoœci (0-100)
    bool startGame;
    bool exitGame;
    bool isGameOver = false; 
    bool enteringNames = false; //  Czy jesteœmy w trybie wpisywania?
    int currentStep = 0;  //0-menu, 1-nick1, 2-nick2
    bool namesEnteredOnce = false; //  blokada ponownego wpisywania nickow
};