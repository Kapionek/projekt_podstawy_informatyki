#include <SFML/Graphics.hpp>  // biblioteka SFML

struct Object { //podstawowa struktura
	sf::CircleShape sprite; 
	sf::Vector2f velocity;
	sf::Color color;
	bool movable = true;
	virtual void _physics_process(float delta) { // ta funcja dzieje się co klatkę i każda struktura 
		if (movable) {							// która jest Object może mieć ruch
			sprite.move(velocity * delta);
		}
	}
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka

	//inicjacja obiektów 
	Object pilka;  // tu nazywasz obiekt i piszesz co to za obiekt ( narazie wszystko z Object bo będzie najłatwiej )
	pilka.color = sf::Color::Red;  // tu zmieniasz wartości tych zmiennych z góry 
	pilka.sprite = sf::CircleShape(5.f);
	pilka.sprite.setFillColor(pilka.color);
	pilka.sprite.setPosition({400.f, 400.f});
	pilka.velocity = { 100.f, 150.f };	

	Object pilka2;
	pilka2.color = sf::Color::Blue;
	pilka2.sprite = sf::CircleShape(5.f);
	pilka2.sprite.setFillColor(pilka2.color);
	pilka2.sprite.setPosition({200.f, 200.f});
	pilka2.velocity = { -120.f, 80.f };

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time dt = clock.restart(); // mierzy czas między klatkami
        float delta = dt.asSeconds(); 
		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
            if (event->is<sf::Event::Closed>()) 
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
        }
		// tu _physic_process()
		pilka._physics_process(delta);
		pilka2._physics_process(delta);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		window.draw(pilka.sprite);
		window.draw(pilka2.sprite);
		window.display(); // wyświetla zawartość okna
    }
    return 0;
}