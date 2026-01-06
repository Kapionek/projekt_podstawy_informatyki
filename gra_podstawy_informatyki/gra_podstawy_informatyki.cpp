#include <SFML/Graphics.hpp>  // biblioteka SFML
#include <vector>  // to jest do vectorów 
#include "obiekty.h"
#include "bullet.h"
#include "sterowanie.h"

int main() {
	sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka
	const int max_bullets = 10; // const jest potrzebny do tablicy  

	// inicjacja textur (musmy je zainicjować wcześniej bo SFML musi mieć textury wcześniej 

	sf::Texture textura;    // tu jest ładowanie textur 
	if (!textura.loadFromFile("textury/pilka.png")) {
		return -1;
	}  // ogólnie to do każdej textury robimy dokładnie to samo tylko zmieniasz nazwę i ścierzkę do pliku 
	sf::Texture textura_sciana;
	if (!textura_sciana.loadFromFile("textury/sciana2.png")) {
		return -1;
	}

	// inicjacja obiektów

	std::vector<Bullet> bullets(max_bullets, Bullet(textura));  // zmieniłem na vector bo z normalnymi tablicami mi errory wyskakiwały 
	std::vector<Object> walls(10, Object(textura_sciana)); // jak coś to w " < > " piszesz rodzaj struktury a w nawiasie ( ilość, obiekt)  

	for (int i = 0; i < max_bullets; i++) { // tu masz
		bullets[i].visible = false;
		bullets[i].accel = -50.f;
		bullets[i].sprite.setOrigin({ 8,8 }); // jako że ten sprite jest 16x16 to tu ustawiasz "środek" na 8x8
	}

	for (int i = 0; i < 10; i++) {
		walls[i].movable = false;
		walls[i].sprite.setOrigin({ 8,8 });
		walls[i].sprite.setPosition({ float(16 * i + 100), float(400) });
	}

	Object gracz(textura); // tu nazywasz obiekt i piszesz co to za obiekt 
	gracz.accel = -250.f;
	gracz.sprite.setOrigin({ 8,8 });
	gracz.sprite.setPosition({ 100.f, 100.f });

	Object gracz2(textura);
	gracz2.accel = -250.f;
	gracz2.sprite.setOrigin({ 8,8 });
	gracz2.sprite.setPosition({ 100.f, 100.f });

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time timer = clock.restart(); // sf::Time - to jest wynik // mierzy czas między klatkami
		float delta = timer.asSeconds();

		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
			if (event->is<sf::Event::Closed>())
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
		} // tu nic nie zmieniamy ^

		obslugaGracza1(gracz, bullets.data(), max_bullets); // dałem .data bo bez .data to nie działa bo to inny rodzaj tablicy
		obslugaGracza2(gracz2, bullets.data(), max_bullets);

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