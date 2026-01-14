#include <SFML/Graphics.hpp>  // biblioteka SFML
#include <vector>  // to jest do vectorów 
#include <ctime>
#include <cstdlib>
#include "obiekty.h"
#include "bullet.h"
#include "sterowanie.h"
#include "zycie.h"
#include "Menu.h"



enum class GameState //enum zarzadza stanami gry w tym samym oknie 
{
	MENU,
	GAME
};

int main() {
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode({ 800,808 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka
	const int max_bullets = 20; // const jest potrzebny do tablicy  

	// inicjacja textur (musmy je zainicjować wcześniej bo SFML musi mieć textury wcześniej 

	sf::Texture textura_speed_buff;
	if (!textura_speed_buff.loadFromFile("textury/speed_buff.png")) {
		return -1;
	}
	sf::Texture textura_speed_debuff;
	if (!textura_speed_debuff.loadFromFile("textury/speed_debuff.png")) {
		return -1;
	}
	sf::Texture textura_multishot;
	if (!textura_multishot.loadFromFile("textury/multishot.png")) {
		return -1;
	}
	sf::Texture textura_big_ball;
	if (!textura_big_ball.loadFromFile("textury/big_ball.png")) {
		return -1;
	}
	sf::Texture textura_zycie;
	if (!textura_zycie.loadFromFile("textury/zycie.png")) {
		return -1;
	}
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
	std::vector<Object> walls(250, Object(textura_sciana)); // jak coś to w " < > " piszesz rodzaj struktury a w nawiasie ( ilość, obiekt)  
	std::vector<Zycie> zycia(6, Zycie(textura_zycie));

	for (int i = 0; i < max_bullets; i++) { // tu masz
		bullets[i].visible = false;
		bullets[i].accel = -20.f;
		bullets[i].hard_accel = -20.f;
		bullets[i].sprite.setOrigin({ 8,8 }); // jako że ten sprite jest 16x16 to tu ustawiasz "środek" na 8x8
		bullets[i].max_speed = 500;
	}
	for (int i = 0; i < 50; i++) {
		float pos = i * 16.f + 8.0f;//16 pikseli szerokośći oraz środek w punkcie (8,8)
		walls[i].sprite.setPosition({ pos, 16.f }); //góra
		walls[i + 50].sprite.setPosition({ pos, 800.f });//dół
		walls[i + 100].sprite.setPosition({ 8.f, pos + 8});//lewo
		walls[i + 150].sprite.setPosition({ 792.f,pos + 8});//prawo
	}

	for (int i = 0; i < 25; i++) {
		float offset = i * 16.f;
		walls[i + 200].sprite.setPosition({ 200.f + offset, 200.f });
		walls[i + 225].sprite.setPosition({ 200.f + offset, 600.f });
	}

	for (int i = 0; i < 250; i++) {
		walls[i].movable = false;
		walls[i].sprite.setOrigin({ 8,8 });
		
	}

	GameState state = GameState::MENU; // tworzenie menu
	Menu menu(window, walls);

	Object gracz(textura_gracz); // tu nazywasz obiekt i piszesz co to za obiekt 
	gracz.accel = -250.f;
	gracz.sprite.setOrigin({ 10,10 });
	gracz.sprite.setPosition({ 100.f, 100.f });
	gracz.team = 1;
	gracz.rotatable = true;
	gracz.damagable = true;
	gracz.buffable = true;

	Object gracz2(textura_gracz);
	gracz2.accel = -250.f;
	gracz2.sprite.setOrigin({ 10,10 });
	gracz2.sprite.setPosition({ 700.f, 700.f });
	gracz2.team = 2; 
	gracz2.rotatable = true;
	gracz2.damagable = true;
	gracz2.buffable = true;

	Object speedup(textura_speed_buff);
	speedup.sprite.setPosition({ (float)(rand()%600+100), (float)(rand()%600 +100)});
	speedup.sprite.setOrigin({ 8,8 });
	speedup.sprite.setColor(sf::Color::Green);
	speedup.visible = true;

	Object slowdown(textura_speed_debuff);
	slowdown.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	slowdown.sprite.setOrigin({ 8,8 });
	slowdown.sprite.setColor(sf::Color::Red);
	slowdown.visible = true;

	Object big_ball(textura_big_ball);
	big_ball.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	big_ball.sprite.setOrigin({ 8,8 });
	big_ball.sprite.setColor(sf::Color::Green);
	big_ball.visible = true;

	Object pdouble(textura_multishot);
	pdouble.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	pdouble.sprite.setOrigin({ 8,8 });
	pdouble.sprite.setColor(sf::Color::Red);
	pdouble.visible = true;


	for (int i = 0; i < 6; i++) {
		zycia[i].movable = false;
		zycia[i].sprite.setOrigin({ 4,4 });
		float pos = i * 8.f + 4.f;
		if (i < 3) {
			zycia[i].sprite.setPosition({ pos, 4.f });
			zycia[i].team = 1;
			zycia[i].gracz = &gracz;
			zycia[i].id = i;
		}
		else {
			zycia[i].sprite.setPosition({ pos + 752, 4.f });
			zycia[i].team = 2;
			zycia[i].gracz = &gracz2;
			zycia[i].id = i - 3;
		}
	}

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	float respawn_timer = 0.f; //licznik czasu do odrodzenia bonusów
	float respawn_time_limit = 10.f; //wracanie bonusów
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte


		//wywołaie menu 
		if (state == GameState::MENU) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) window.close();
				menu.handleEvent(*event);
			}

			window.clear();
			menu.draw();
			window.display();

			if (menu.shouldStartGame()) state = GameState::GAME;
			if (menu.shouldExit()) window.close();

			continue; // przeskakkuje do logiki gry
		}


		sf::Time timer = clock.restart(); // sf::Time - to jest wynik // mierzy czas między klatkami
		float delta = timer.asSeconds();

		//Respawn bonusów
		respawn_timer += delta;
		if (respawn_timer >= respawn_time_limit) {
			//sprawdzanie każdego bonusu
			if (speedup.visible == false) {
				speedup.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				speedup.visible = true;
			}
			if (slowdown.visible == false) {
				slowdown.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				slowdown.visible = true;
			}
			if (big_ball.visible == false) {
				big_ball.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				big_ball.visible = true;
			}
			if (pdouble.visible == false) {
				pdouble.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				pdouble.visible = true;
			}
			respawn_timer = 0.f; //reset licznika po każdej próbie odrodzenia
		}

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
		
		for (int i = 0; i < 6; i++) {
			zycia[i]._physics_process(delta);
		}

		//KOLIZJE
		
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				for (int j = 0; j < 250; j++) {
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
					gracz.health -= 1; 
					bullets[i].visible = false;
				}
				if (bullets[i].sprawdzKolizje(gracz2) and bullets[i].team != gracz2.team) {
					gracz2.health -= 1;
					bullets[i].visible = false;
				}
			}
		}

		//ślizganie graczy na ścianach
		for (int j = 0; j < 250; j++) {
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
					gracz.velocity.y *= 0.8f; //tarcie (spowolnienie)
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
					gracz.velocity.x *= 0.8f; //tarcie (spowolnienie)
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
		if (speedup.visible) {
			//gracz1 z przyśpieszeniem
			if (gracz.sprawdzKolizje(speedup)) {
				speedup.visible = false;
				gracz.apply_buffs(0);
			}
			//gracz2 z przyśpieszeniem
			if (gracz2.sprawdzKolizje(speedup)) {
				speedup.visible = false;
				gracz2.apply_buffs(0);
			}
		}
		if (slowdown.visible) {
			//gracz1 ze spowolnieniem
			if (gracz.sprawdzKolizje(slowdown)) {
				gracz2.apply_buffs(1);
				slowdown.visible = false;
			}
			//gracz2 ze spowolnieniem
			if (gracz2.sprawdzKolizje(slowdown)) {
				gracz.apply_buffs(1);
				slowdown.visible = false;
			}
		}
		if (big_ball.visible) {
			//gracz1 z powiększniem kulki
			if (gracz.sprawdzKolizje(big_ball)) {
				gracz.apply_buffs(2);
				big_ball.visible = false;
			}
			//gracz2 z powiększniem kulki
			if (gracz2.sprawdzKolizje(big_ball)) {
				gracz2.apply_buffs(2);
				big_ball.visible = false;
			}
		}
		if (pdouble.visible) {
			//gracz 1 z podwójnym strzałem
			if (gracz.sprawdzKolizje(pdouble)) {
				gracz.apply_buffs(3);
				pdouble.visible = false;
			}
		//gracz 2 z podwójnym strzałem
			if (gracz2.sprawdzKolizje(pdouble)) {
				gracz2.apply_buffs(3);
				pdouble.visible = false;
			}
		}
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		for (int i = 0; i < 250; i++) {
			if (walls[i].visible) {
				window.draw(walls[i].sprite);
			}
		}
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				window.draw(bullets[i].sprite);
			}
		}
		for (int i = 0; i < 6; i++) {
			if (zycia[i].visible) {
				window.draw(zycia[i].sprite);
			}
		}
		if (gracz.visible) {
			window.draw(gracz.sprite);
		}
		if (gracz2.visible) {
			window.draw(gracz2.sprite);
		}
		if (speedup.visible) {
			window.draw(speedup.sprite);
		}
		if (slowdown.visible) {
			window.draw(slowdown.sprite);
		}
		if (big_ball.visible) {
			window.draw(big_ball.sprite);
		}
		if (pdouble.visible) {
			window.draw(pdouble.sprite);
		}

		window.display(); // wyświetla zawartość okna
	}
	return 0;
}