#include <SFML/Graphics.hpp>  // biblioteka SFML

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka 
	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time dt = clock.restart(); // mierzy czas między klatkami
        float delta = dt.asSeconds(); 
		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
            if (event->is<sf::Event::Closed>()) 
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
        }
		window.clear();  // czyści okno z poprzedniej klatki
		window.display(); // wyświetla zawartość okna
    }
    return 0;
}