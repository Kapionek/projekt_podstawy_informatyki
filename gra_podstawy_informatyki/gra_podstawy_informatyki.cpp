#include <SFML/Graphics.hpp>  // biblioteka SFML

struct Object { //podstawowa struktura
	sf::CircleShape sprite; 
	sf::Vector2f direction;
	sf::Vector2f velocity;
	sf::Color color;
	int speed = 100;
	bool movable = true;
	virtual void _physics_process(float delta) { // ta funcja dzieje się co klatkę i każda struktura moze urzyć tej funcji 
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length != 0) { // aby na ukos nie biegał szybciej 
			direction.x /= length;
			direction.y /= length;
		}
		if (movable) {							// która jest Object może mieć ruch
			velocity.y = direction.y * speed;
			velocity.x = direction.x * speed;
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
	pilka.direction = { 1.f, 1.f };
	pilka.speed = 150;

	Object pilka2;
	pilka2.color = sf::Color::Blue;
	pilka2.sprite = sf::CircleShape(5.f);
	pilka2.sprite.setFillColor(pilka2.color);
	pilka2.sprite.setPosition({200.f, 200.f});
	pilka2.direction = { 0.9f, 1.f };
	pilka2.speed = 120;

	Object gracz;
	gracz.color = sf::Color::Cyan;
	gracz.sprite = sf::CircleShape(10.f);
	gracz.sprite.setFillColor(gracz.color);
	gracz.sprite.setPosition({ 100.f, 100.f });

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time dt = clock.restart(); // mierzy czas między klatkami
        float delta = dt.asSeconds(); 
		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
            if (event->is<sf::Event::Closed>()) 
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
			gracz.direction = { 0.f, 0.f };
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //tu jest ogarniani unputu z klawiatury
				gracz.direction.y = -1;//to jest specjalnie bo współrzędne y-kowe są na odwrót. x -owe są normalnie jak coś 
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
				gracz.direction.y = 1; 
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
				gracz.direction.x = -1; 
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
				gracz.direction.x = 1;
			}
        }
		// tu _physic_process()
		pilka._physics_process(delta);
		pilka2._physics_process(delta);
		gracz._physics_process(delta);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		window.draw(pilka.sprite);
		window.draw(pilka2.sprite);
		window.draw(gracz.sprite);
		window.display(); // wyświetla zawartość okna
    }
    return 0;
}