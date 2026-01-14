#include "Menu.h"


// Dodaje playText i exitText do listy inicjalizacyjnej
Menu::Menu(sf::RenderWindow& window, std::vector<Object>& walls)
    : window(window),
    wallsRef(walls),
    selected(0),
    startGame(false),
    exitGame(false),
    playText(font), // Inicjalizacja tekstu czcionk¹ 
    exitText(font),
    resultText(font)
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


    resultText.setFont(font);
    resultText.setCharacterSize(60);
    resultText.setFillColor(sf::Color::Yellow);
    resultText.setPosition({ 150.f, 150.f }); // Wy¿ej ni¿ przyciski
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
    // 1. Rysowanie t³a (œcian)
    for (auto& wall : wallsRef) {
        window.draw(wall.sprite);
    }

    if (isGameOver) {
        // Ciemna nak³adka na ca³y ekran
        sf::RectangleShape overlay({ 800.f, 808.f });
        overlay.setFillColor(sf::Color(0, 0, 0, 180)); // Czarny, 70% widocznoœci
        window.draw(overlay);

        window.draw(resultText);
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

void Menu::setEndScreen(int winner) {
    isGameOver = true;
    resultText.setString("GRACZ " + std::to_string(winner) + " WYGRAL!");
    playText.setString("ZAGRAJ PONOWNIE");
    exitText.setString("WYJDZ DO PULPITU");

    // Funkcja pomocnicza do œrodkowania (wywo³ujemy dla ka¿dego tekstu)
    auto centerText = [&](sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        // Ustawiamy punkt obrotu/œrodek tekstu na jego geometryczny œrodek
        text.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
        // Ustawiamy tekst na œrodku szerokoœci okna (400px)
        text.setPosition({ 400.0f, y });
        };

    centerText(resultText, 250.0f); // Napis kto wygra³
    centerText(playText, 450.0f);   // Przycisk 1
    centerText(exitText, 530.0f);   // Przycisk 2

    updateSelection();
}

void Menu::resetFlags() {
    startGame = false;
    exitGame = false;
    isGameOver = false;

    playText.setString("GRAJ");
    exitText.setString("WYJDZ");

    auto centerText = [&](sf::Text& text, float y) {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
        text.setPosition({ 400.0f, y });
        };

    centerText(playText, 450.0f);
    centerText(exitText, 530.0f);

    updateSelection();
}