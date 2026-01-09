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
	if (!textura_sciana.loadFromFile("textury/sciana.png")) {
		return -1;
	}
	sf::Texture textura_gracz;
	if (!textura_gracz.loadFromFile("textury/gracz.png")) {
		return -1;
	}

	// inicjacja obiektów

	std::vector<Bullet> bullets(max_bullets, Bullet(textura));  // zmieniłem na vector bo z normalnymi tablicami mi errory wyskakiwały 
	std::vector<Object> walls(200, Object(textura_sciana)); // jak coś to w " < > " piszesz rodzaj struktury a w nawiasie ( ilość, obiekt)  

	for (int i = 0; i < max_bullets; i++) { // tu masz
		bullets[i].visible = false;
		bullets[i].accel = -20.f;
		bullets[i].sprite.setOrigin({ 8,8 }); // jako że ten sprite jest 16x16 to tu ustawiasz "środek" na 8x8
		bullets[i].max_speed = 500;
	}
	for (int i = 0; i < 50; i++) {
		float pos = i * 16.f + 8.0f;//16 pikseli szerokośći oraz środek w punkcie (8,8)
		walls[i].sprite.setPosition({ pos, 8.f }); //góra
		walls[i + 50].sprite.setPosition({ pos, 792.f });//dół
		walls[i + 100].sprite.setPosition({ 8.f, pos });//lewo
		walls[i + 150].sprite.setPosition({ 792.f,pos });//prawo
	}

	for (int i = 0; i < 200; i++) {
		walls[i].movable = false;
		walls[i].sprite.setOrigin({ 8,8 });
		
	}

	Object gracz(textura_gracz); // tu nazywasz obiekt i piszesz co to za obiekt 
	gracz.accel = -250.f;
	gracz.sprite.setOrigin({ 8,8 });
	gracz.sprite.setPosition({ 100.f, 100.f });
	gracz.team = 1;
	gracz.rotatable = true;

	Object gracz2(textura_gracz);
	gracz2.accel = -250.f;
	gracz2.sprite.setOrigin({ 8,8 });
	gracz2.sprite.setPosition({ 700.f, 700.f });
	gracz2.team = 2; 
	gracz2.rotatable = true;

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

		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				bullets[i]._physics_process(delta);
			}
		}

		//KOLIZJE
		
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				for (int j = 0; j < 200; j++) {
					//sprawdzanie, czy zaszła kolizja i pobranie jej obszaru
					auto pole_kolizji = bullets[i].sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds()); 
					//zmienna przechowuje mały prostokąt, czyli wspolny obszar piłki i ściany gdy zachodzi kolizja

					if (pole_kolizji){ //doszło do zderzenia
						if (pole_kolizji->size.x < pole_kolizji->size.y) { //uderzenie z boku (węższe pole kolizji)
							bullets[i].direction.x *= -1.0f; // odwracanie kierunku poziomego
						}
						else {
							bullets[i].direction.y *= -1.0f;
						}
						bullets[i].sprite.move(bullets[i].direction * 2.0f);
					}
				}
				// kolizja z graczami
				
				if (bullets[i].sprawdzKolizje(gracz) and bullets[i].team != gracz.team) {
					gracz.visible = false; 
					bullets[i].visible = false;
				}
				if (bullets[i].sprawdzKolizje(gracz2) and bullets[i].team != gracz2.team) {
					gracz2.visible = false; 
					bullets[i].visible = false;
				}
			}
		}

		//ślizganie graczy na ścianach
		for (int j = 0; j < 200; j++) {
			//kolizje dla 1 gracza
			auto pole_g1 = gracz.sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds());
			if (pole_g1) {
				if (pole_g1->size.x < pole_g1->size.y) {//kolizja pozioma
					float kierunek;
					if (gracz.sprite.getPosition().x < walls[j].sprite.getPosition().x) {
						kierunek = -1.0f; // wypchany w lewo, gdyż gracz jest po lewej stronie ściany
					}
					else {
						kierunek = 1.0f;
					}
					gracz.sprite.move({ pole_g1->size.x * kierunek, 0.0f });
					gracz.velocity.x = 0; //tylko bieg góra/dół
					gracz2.velocity.y *= 0.8f; //tarcie (spowolnienie)
				}
				else {//kolizja pionowa
					float kierunek;
					if (gracz.sprite.getPosition().y < walls[j].sprite.getPosition().y) {
						kierunek = -1.0f; // wypchany w górę, gdyż gracz jest nad ścianą
					}
					else {
						kierunek = 1.0f;
					}
					gracz.sprite.move({ 0.0f, pole_g1->size.y * kierunek });
					gracz.velocity.y = 0; //tylko bieg prawo/lewo
					gracz2.velocity.x *= 0.8f; //tarcie (spowolnienie)
				}
			}
			auto pole_g2 = gracz2.sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds());
			if (pole_g2) {
				if (pole_g2->size.x < pole_g2->size.y) {//kolizja pozioma
					float kierunek;
					if (gracz2.sprite.getPosition().x < walls[j].sprite.getPosition().x) {
						kierunek = -1.0f; // wypchany w lewo, gdyż gracz jest po prawej stronie ściany
					}
					else {
						kierunek = 1.0f;
					}
					gracz2.sprite.move({ pole_g2->size.x * kierunek, 0.0f });
					gracz2.velocity.x = 0; //tylko bieg góra/dół
					gracz2.velocity.y *= 0.8f; //tarcie (spowolnienie)
				}
				else {//kolizja pionowa
					float kierunek;
					if (gracz2.sprite.getPosition().y < walls[j].sprite.getPosition().y) {
						kierunek = -1.0f; // wypchany w górę, gdyż gracz jest nad ścianą
					}
					else {
						kierunek = 1.0f;
					}
					gracz2.sprite.move({ 0.0f, pole_g2->size.y * kierunek });
					gracz2.velocity.y = 0; //tylko bieg prawo/lewo
					gracz2.velocity.x *= 0.8f; //tarcie (spowolnienie)
				}
			}
		}
		//kolizja między 1 a 2 graczem
		//sprawdzanie czy gracze na siebie nachodzą, jeśli tak to tworzy się ich część wspólna
		auto pole_zderzenia = gracz.sprite.getGlobalBounds().findIntersection(gracz2.sprite.getGlobalBounds());
		if (pole_zderzenia) {//gracze się zderzyli
			if (pole_zderzenia->size.x < pole_zderzenia->size.y) {//jeśli prostokąt kolizji jest węższy niż wyższy to zderzyli się poziomo
				//kolizja pozioma
				float kierunek;
				if (gracz.sprite.getPosition().x < gracz2.sprite.getPosition().x) {
					kierunek = -1.0f; //gracz 1 jest po lewej, więc wypycha w lewo
				}
				else {
					kierunek = 1.0f; //gracz 1 jest z prawej więc wypycha w prawo
				}
				gracz.sprite.move({ (pole_zderzenia->size.x / 2.0f) * kierunek,0.0f }); //głębokość zderzenie dzielona na pół i przesunięcie obu w przeciwnych kierunkach
				gracz2.sprite.move({ (pole_zderzenia->size.x / 2.0f) * -kierunek,0.0f });

				gracz.velocity.x = 0;//zerowanie prędkości w osi poziomej czyli tej w której jest zderzenie w tym przypadku, prędkość y bez zmian
				gracz2.velocity.x = 0;
			}
			else {
				//kolizja pionowa
				float kierunek;
				if (gracz.sprite.getPosition().y < gracz2.sprite.getPosition().y) {
					kierunek = -1.0f; //gracz 1 jest nad graczem 2, więc wypycha w górę
				}
				else {
					kierunek = 1.0f; //gracz 1 jest pod graczem 2, więc wypycha w dół
				}
				gracz.sprite.move({0.0f, (pole_zderzenia->size.y / 2.0f) * kierunek});
				gracz2.sprite.move({0.0f, (pole_zderzenia->size.y / 2.0f) * -kierunek});

				gracz.velocity.y = 0;
				gracz2.velocity.y = 0;
			}
		}
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		for (int i = 0; i < 200; i++) {
			if (walls[i].visible) {
				window.draw(walls[i].sprite);
			}
		}
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
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