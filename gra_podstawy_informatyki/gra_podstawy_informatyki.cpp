#include <SFML/Graphics.hpp>  // biblioteka SFML
#include "obiekty.h"
#include "bullet.h"
#include "sterowanie.h"

int main() {
	sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka
	const int max_bullets = 10; // const jest potrzebny do tablicy  

	// inicjacja obiektów

	Bullet bullets[10]; // tablica na pociski
	Object walls[10];
	for (int i = 0; i < max_bullets; i++) {
		bullets[i].visible = false;
		bullets[i].sprite = sf::CircleShape(3.f);
		bullets[i].color = sf::Color::Yellow;
		bullets[i].sprite.setFillColor(sf::Color::Yellow);
		bullets[i].accel = -50.f;
	}

	for (int i = 0; i < 10; i++) {
		walls[i].movable = false;
		walls[i].color = sf::Color::Red;
		walls[i].sprite = sf::CircleShape(10.f);
		walls[i].sprite.setFillColor(sf::Color::Red);
		walls[i].sprite.setPosition({ float(10 * i + 100), float(400) });
	}

	Object gracz; // tu nazywasz obiekt i piszesz co to za obiekt ( narazie wszystko z Object bo będzie najłatwiej )
	gracz.color = sf::Color::Cyan;  // tu zmieniasz wartości tych zmiennych z góry z struktury
	gracz.sprite = sf::CircleShape(10.f);
	gracz.sprite.setFillColor(gracz.color);
	gracz.sprite.setPosition({ 100.f, 100.f });
	gracz.accel = -250.f;

	Object gracz2;
	gracz2.color = sf::Color::Cyan;  
	gracz2.sprite = sf::CircleShape(10.f);
	gracz2.sprite.setFillColor(gracz2.color);
	gracz2.sprite.setPosition({ 100.f, 100.f });
	gracz2.accel = -250.f;

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time dt = clock.restart(); // mierzy czas między klatkami
		float delta = dt.asSeconds();

		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
			if (event->is<sf::Event::Closed>())
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
		} // tu nic nie zmieniamy ^

		obslugaGracza1(gracz, bullets, max_bullets);
		obslugaGracza2(gracz2, bullets, max_bullets);


		// tu _physic_process()
		gracz._physics_process(delta);
		gracz2._physics_process(delta);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		for (int i = 0; i < 10; i++) {
			if (walls[i].visible) {
				window.draw(walls[i].sprite);
			}
		}
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				bullets[i]._physics_process(delta);
				window.draw(bullets[i].sprite);
			}
		}
		if (gracz.visible) {
			window.draw(gracz.sprite);
		}
		if (gracz2.visible) {
			window.draw(gracz2.sprite);
		}
		window.display(); // wyświetla zawartość okna
	}
	return 0;
}