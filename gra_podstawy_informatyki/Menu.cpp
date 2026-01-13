#include "Menu.h"


// Dodaje playText i exitText do listy inicjalizacyjnej
Menu::Menu(sf::RenderWindow& window, std::vector<Object>& walls)
    : window(window),
    wallsRef(walls),
    selected(0),
    startGame(false),
    exitGame(false),
    playText(font), // Inicjalizacja tekstu czcionk¹ 
    exitText(font)
{
    //£adowanie czcionki z pliku
    if (!font.openFromFile("arial.ttf")) {
        
    }

    //Konfiguracja tekstów 
    playText.setString("GRAJ");
    playText.setCharacterSize(48);
    playText.setPosition({ 338.f, 300.f });

    exitText.setString("WYJDZ");
    exitText.setCharacterSize(48);
    exitText.setPosition({ 320.f, 380.f });

    // Ustawienie pocz¹tkowych kolorów
    updateSelection();
}

void Menu::handleEvent(const sf::Event& event) {
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Up) {
            selected = 0;
            updateSelection();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Down) {
            selected = 1;
            updateSelection();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Enter) {
            if (selected == 0) {
                startGame = true;
            }
            else {
                exitGame = true;
            }
        }
    }
}

void Menu::updateSelection() {
    playText.setFillColor(selected == 0 ? sf::Color::Red : sf::Color::White);
    exitText.setFillColor(selected == 1 ? sf::Color::Red : sf::Color::White);
}

void Menu::draw() {
    for (auto& wall : wallsRef) {
        window.draw(wall.sprite);
    }

    window.draw(playText);
    window.draw(exitText);
}

bool Menu::shouldStartGame() const {
    return startGame;
}

bool Menu::shouldExit() const {
    return exitGame;
}


