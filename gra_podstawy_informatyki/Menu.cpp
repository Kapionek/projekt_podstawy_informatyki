#include "Menu.h"

Menu::Menu(sf::RenderWindow& window, std::vector<Object>& walls)
    : window(window),
    wallsRef(walls),
    selected(0),
    startGame(false),
    exitGame(false),
    playText(font),
    exitText(font),
    resultText(font),
    volumeText(font),
    promptText(font),
    inputDisplay(font),
    namesEnteredOnce(false) // Inicjalizacja flagi
{
    if (!font.openFromFile("assets/arial.ttf")) {
        // obs³uga b³êdu
    }

    // Konfiguracja g³oœnoœci
    playText.setCharacterSize(40);
    volumeText.setCharacterSize(40);
    exitText.setCharacterSize(40);

    // Konfiguracja wpisywania nicków
    promptText.setCharacterSize(40);
    promptText.setFillColor(sf::Color::Cyan);
    inputDisplay.setCharacterSize(50);
    inputDisplay.setFillColor(sf::Color::White);

    resultText.setCharacterSize(60);
    resultText.setFillColor(sf::Color::Yellow);

    resetFlags();
}

void Menu::handleEvent(const sf::Event& event) {
    if (enteringNames) {
        if (const auto* textEntered = event.getIf<sf::Event::TextEntered>()) {
            // Obs³uga Backspace
            if (textEntered->unicode == '\b') {
                if (!currentInput.empty()) currentInput.pop_back();
            }
            // Obs³uga Entera - przeskakiwa³o mi opcje wpisywania nicku dla gracza 1 bo wczytywa³o wczesniejszego entera
            else if (textEntered->unicode == '\r' || textEntered->unicode == '\n') {
                if (!currentInput.empty()) { // Nie pozwalamy na pusty nick, ¿eby unikn¹æ b³êdów
                    if (currentStep == 1) {
                        nick1 = currentInput;
                        currentInput.clear();
                        inputDisplay.setString(""); // czyœci wyœwietlany tekst
                        currentStep = 2;            // Przechodzi do kroku 2
                        return; // - to zapobiega przeskoczeniu do gracza 2
                    }
                    else if (currentStep == 2) {
                        nick2 = currentInput;
                        currentInput.clear();
                        enteringNames = false;
                        namesEnteredOnce = true;
                        startGame = true;
                        return;
                    }
                }
            }
            // Wpisywanie znaków
            else if (textEntered->unicode < 128 && textEntered->unicode > 31) {
                currentInput += static_cast<char>(textEntered->unicode);
            }

            inputDisplay.setString(currentInput);
            centerText(inputDisplay, 400.f);
        }
        return; // Blokujemy resztê zdarzeñ (np. strza³ki) podczas pisania
    }

    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        //  Obs³uga 3 opcji (0, 1, 2) 
        if (keyPressed->code == sf::Keyboard::Key::Up) {
            selected = (selected - 1 + 3) % 3; // tu zamienilem + z -
            updateSelection();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Down) {
            selected = (selected + 1) % 3; //tu na odwrot
            updateSelection();
        }
        //  obsluga zmiany g³oœnoœci strza³kami boki
        else if (selected == 1 && keyPressed->code == sf::Keyboard::Key::Left) {
            volume = std::max(0.f, volume - 5.f);
            updateSelection();
        }
        else if (selected == 1 && keyPressed->code == sf::Keyboard::Key::Right) {
            volume = std::min(100.f, volume + 5.f);
            updateSelection();
        }
        else if (keyPressed->code == sf::Keyboard::Key::Enter) {
            if (selected == 0) {
                // nicki wpisujesie tylko przy pierwszym odpaleniu gryy
                if (!namesEnteredOnce) {
                    enteringNames = true;
                    currentStep = 1;
                    currentInput.clear();
                    inputDisplay.setString("");
                }
                else {
                    startGame = true; // Jeœli ju¿ wpisane, po prostu startuj
                }
            }
            /*else if (selected == 1) {
                // Opcja g³oœnoœci obs³ugiwana strza³kami boki
            }*/
            else if (selected == 2) {
                exitGame = true;
            }
        }
    }
}

void Menu::updateSelection() {
    volumeText.setString("GLOSNOSC: " + std::to_string((int)volume) + "%");

    volumeText.setCharacterSize(40);
    playText.setCharacterSize(40);
    exitText.setCharacterSize(40);

    // Ustawiamy pozycjê g³oœnoœci zale¿nie od tego czy to koniec gry
    float volY = isGameOver ? 450.0f : 400.f;
    centerText(volumeText, volY); //centruje bo tekst zmienil dlugosc

    // Aktualizacja kolorów dla wszystkich 3 przycisków
    playText.setFillColor(selected == 0 ? sf::Color::Red : sf::Color::White);
    volumeText.setFillColor(selected == 1 ? sf::Color::Red : sf::Color::White);
    exitText.setFillColor(selected == 2 ? sf::Color::Red : sf::Color::White);
}

void Menu::draw() {
    for (auto& wall : wallsRef) {
        window.draw(wall.sprite);
    }

    if (enteringNames) {
        sf::RectangleShape overlay({ 800.f, 816.f });
        overlay.setFillColor(sf::Color(0, 0, 0, 220));
        window.draw(overlay);

        promptText.setString("WPISZ NICK GRACZA " + std::to_string(currentStep) + ":");
        centerText(promptText, 300.f);

        window.draw(promptText);
        window.draw(inputDisplay);
    }
    else {
        if (isGameOver) {
            sf::RectangleShape overlay({ 800.f, 808.f });
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);
            window.draw(resultText);
        }

        window.draw(playText);
        window.draw(volumeText);
        window.draw(exitText);
    }
}

void Menu::setEndScreen(int winner) {
    isGameOver = true;
    selected = 0; // Resetujemy wybór na pierwszy przycisk

    // Wyœwietlanie nicku zamiast numeru gracza
    std::string winnerName = (winner == 1) ? nick1 : nick2;
    resultText.setString("WYGRAL " + winnerName + "!");

    playText.setString("ZAGRAJ PONOWNIE");
    exitText.setString("WYJDZ DO PULPITU");

    // Poprawione rozmieszczenie, aby g³oœnoœæ (450.0f) by³a pomiêdzy przyciskami
    centerText(resultText, 250.0f);
    centerText(playText, 370.0f);
    centerText(volumeText, 450.0f); // Dodane wyœrodkowanie g³oœnoœci w menu koñcowym
    centerText(exitText, 530.0f);

    updateSelection();
}

void Menu::resetFlags() {
    startGame = false;
    exitGame = false;
    isGameOver = false;
    enteringNames = false;
    currentStep = 0;
    selected = 0;

    playText.setString("GRAJ");
    exitText.setString("WYJDZ");

    //  jednolity centerText dla porz¹dku
    centerText(playText, 320.0f);
    centerText(volumeText, 400.f);
    centerText(exitText, 480.0f);
    updateSelection();
}

void Menu::centerText(sf::Text& text, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.size.x / 2.0f, bounds.size.y / 2.0f });
    text.setPosition({ 400.0f, y });
}

bool Menu::shouldStartGame() const {
    return startGame;
}

bool Menu::shouldExit() const {
    return exitGame;
}