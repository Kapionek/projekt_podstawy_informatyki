#include <SFML/Graphics.hpp>  // biblioteka SFML
#include "obiekty.h"
#include "bullet.h"

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

		if (gracz.speed <= 0.f) { // ta część obsługuje gracza i też zwalnianie
			gracz.direction = { 0.f, 0.f };
		}
		gracz.max_speed_on = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //tu jest ogarniani unputu z klawiatury
			if (gracz.speed <= gracz.max_speed) {  // ten warunek sprawia że nie oddziałowywuje gdy dashujesz przez co nie możesz zminiać kierunku dasha
				gracz.direction.y = -1; //to jest specjalnie bo współrzędne y-kowe są na odwrót. x -owe są normalnie jak coś 
				gracz.max_speed_on = true; // dla 4 inputów (WSAD)
				gracz.speed = gracz.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			if (gracz.speed <= gracz.max_speed) {
				gracz.direction.y = 1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			if (gracz.speed <= gracz.max_speed) {
				gracz.direction.x = -1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			if (gracz.speed <= gracz.max_speed) {
				gracz.direction.x = 1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {  // tu jest dash 
			if (gracz.dash_cooldown <= 0.f) {
				gracz.speed = 350.f;
				gracz.dash_cooldown = 0.5;
				gracz.direction = gracz.last_direction; // używa last direction by można było dashować jak sie stoi
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {  //strzał piłką 
			if (gracz.shot_cooldown <= 0.f) {
				for (int i = 0; i < max_bullets; i++) {	 // tu przechodzi przez wszystkie dostępne bullety dostępne 
					if (bullets[i].visible == false) {
						bullets[i].visible = true;  // tu zmienia parametry i ustawia dla tego bulleta 
						bullets[i].sprite.setPosition(gracz.sprite.getPosition());
						bullets[i].direction = gracz.last_direction;
						bullets[i].speed = 400.f;
						bullets[i].life_time = 3.f;
						break; // by nie szukał dalej
					}
				}
				gracz.shot_cooldown = 2;
			}
		}


		if (gracz2.speed <= 0.f) { 
			gracz2.direction = { 0.f, 0.f };
		}
		gracz2.max_speed_on = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) { 
			if (gracz2.speed <= gracz2.max_speed) {  
				gracz2.direction.y = -1; 
				gracz2.max_speed_on = true; 
				gracz2.speed = gracz2.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
			if (gracz2.speed <= gracz2.max_speed) {
				gracz2.direction.y = 1;
				gracz2.max_speed_on = true;
				gracz2.speed = gracz2.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J)) {
			if (gracz2.speed <= gracz2.max_speed) {
				gracz2.direction.x = -1;
				gracz2.max_speed_on = true;
				gracz2.speed = gracz2.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
			if (gracz2.speed <= gracz2.max_speed) {
				gracz2.direction.x = 1;
				gracz2.max_speed_on = true;
				gracz2.speed = gracz2.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
			if (gracz2.dash_cooldown <= 0.f) {
				gracz2.speed = 350.f;
				gracz2.dash_cooldown = 0.5;
				gracz2.direction = gracz2.last_direction;
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::U)) {  
			if (gracz2.shot_cooldown <= 0.f) {
				for (int i = 0; i < max_bullets; i++) {	  
					if (bullets[i].visible == false) {
						bullets[i].visible = true;  
						bullets[i].sprite.setPosition(gracz2.sprite.getPosition());
						bullets[i].direction = gracz2.last_direction;
						bullets[i].speed = 400.f;
						bullets[i].life_time = 3.f;
						break; 
					}
				}
				gracz2.shot_cooldown = 2;
			}
		}
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