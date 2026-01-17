#include <SFML/Graphics.hpp>  // biblioteka SFML
#include <SFML/Audio.hpp> // do dźwięku 
#include <vector>  // to jest do vectorów 
#include <ctime>
#include <cstdlib>
#include "obiekty.h"
#include "bullet.h"
#include "sterowanie.h"
#include "zycie.h"
#include "Menu.h"
#include "kolizje.h"
#include "bonusy.h"



enum class GameState //enum zarzadza stanami gry w tym samym oknie 
{
	MENU,
	GAME,
	END_SCREEN,
	NAME_INPUT
};

//resert gry zrobilem osobna funkcje bo zaczyanlem miec problemy z odnalezieniem sie w kodzie 
void resetujGre(Object& g1, Object& g2, std::vector<Bullet>& b, std::vector<Zycie>& z) {
	// Przywracanie zdrowia i pozycji gracza 1
	g1.health = 3;
	g1.velocity = { 0.f, 0.f };
	g1.sprite.setPosition({ 100.f, 100.f });
	g1.visible = true;

	// Przywracanie zdrowia i pozycji gracza 2
	g2.health = 3;
	g2.velocity = { 0.f, 0.f };
	g2.sprite.setPosition({ 700.f, 700.f });
	g2.visible = true;

	// Ukrywanie wszystkich pocisków na ekranie
	for (auto& bullet : b) {
		bullet.visible = false;
	}

	// Przywracanie widoczności serduszek
	for (auto& serce : z) {
		serce.visible = true;
	}
}

int main(){
	srand(time(NULL));
	sf::RenderWindow window(sf::VideoMode({ 800,816 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka
	const int max_bullets = 20; // const jest potrzebny do tablicy  


	sf::SoundBuffer buffer;
	if (!buffer.loadFromFile("muzyka.mp3")) {
		return -1;
	}
	sf::Sound music(buffer);
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
	if (!textura_zycie.loadFromFile("textury/zycieV2.png")) {
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
		walls[i].sprite.setPosition({ pos, 24.f }); //góra
		walls[i + 50].sprite.setPosition({ pos, 808.f });//dół
		walls[i + 100].sprite.setPosition({ 8.f, pos + 16});//lewo
		walls[i + 150].sprite.setPosition({ 792.f,pos + 16});//prawo
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
	//losowanie pozycji startowej bez ścian
	bool kolizja_start_speedup = true;
	while (kolizja_start_speedup) {
		speedup.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
		kolizja_start_speedup = false;
		for (int i = 0; i < walls.size(); i++) {
			if (speedup.sprawdzKolizje(walls[i])) {
				kolizja_start_speedup = true;
				break;
			}
		}
	}

	Object slowdown(textura_speed_debuff);
	slowdown.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	slowdown.sprite.setOrigin({ 8,8 });
	slowdown.sprite.setColor(sf::Color::Red);
	slowdown.visible = true;
	//losowanie pozycji startowej bez ścian
	bool kolizja_start_slowdown = true;
	while (kolizja_start_slowdown) {
		slowdown.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
		kolizja_start_slowdown = false;
		for (int i = 0; i < walls.size(); i++) {
			if (slowdown.sprawdzKolizje(walls[i])) {
				kolizja_start_slowdown = true;
				break;
			}
		}
	}

	Object big_ball(textura_big_ball);
	big_ball.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	big_ball.sprite.setOrigin({ 8,8 });
	big_ball.sprite.setColor(sf::Color::Green);
	big_ball.visible = true;
	//losowanie pozycji startowej bez ścian
	bool kolizja_start_big_ball = true;
	while (kolizja_start_big_ball) {
		big_ball.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
		kolizja_start_big_ball = false;
		for (int i = 0; i < walls.size(); i++) {
			if (big_ball.sprawdzKolizje(walls[i])) {
				kolizja_start_big_ball = true;
				break;
			}
		}
	}

	Object pdouble(textura_multishot);
	pdouble.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
	pdouble.sprite.setOrigin({ 8,8 });
	pdouble.sprite.setColor(sf::Color::Red);
	pdouble.visible = true;
	//losowanie pozycji startowej bez ścian
	bool kolizja_start_pdouble = true;
	while (kolizja_start_pdouble) {
		pdouble.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
		kolizja_start_pdouble = false;
		for (int i = 0; i < 250; i++) {
			if (pdouble.sprawdzKolizje(walls[i])) {
				kolizja_start_pdouble = true;
				break;
			}
		}
	}


	for (int i = 0; i < 6; i++) {
		zycia[i].movable = false;
		zycia[i].sprite.setOrigin({ 8,8 });
		float pos = i * 16.f + 8.f;
		if (i < 3) {
			zycia[i].sprite.setPosition({ pos, 8.f });
			zycia[i].team = 1;
			zycia[i].gracz = &gracz;
			zycia[i].id = i;
		}
		else {
			zycia[i].sprite.setPosition({ pos + 704, 8.f });
			zycia[i].team = 2;
			zycia[i].gracz = &gracz2;
			zycia[i].id = i - 3;
		}
	}

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	float timer_speedup = 0.f; //liczniki czasu do odrodzenia bonusów
	float timer_slowdown = 0.f;
	float timer_bigball = 0.f;
	float timer_pdouble = 0.f;
	float respawn_time_limit = 10.f; //wracanie bonusów
	music.setLooping(true);
	music.play();
	music.setVolume(2); // tu mamy do ustawiania głośności to w ustawieniach można by zmieniniać 
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		//wywołaie menu 
		if (state == GameState::MENU || state == GameState::END_SCREEN) {
			while (const std::optional event = window.pollEvent()) {
				if (event->is<sf::Event::Closed>()) window.close();
				menu.handleEvent(*event);

				//Dynamiczna aktualizacja głośności muzyki podczas przesuwania paska w menu
				music.setVolume(menu.getVolume());
			}

			if (menu.shouldStartGame()) {
				// resertuje stan gry przed nowym startem
				//Jeśli menu mówi, że gra ma wystartować, upewniamy się, że stan to GAME
					resetujGre(gracz, gracz2, bullets, zycia);
				menu.resetFlags();
				state = GameState::GAME;
				clock.restart();
			}

			if (menu.shouldExit()) window.close();

			window.clear();
			menu.draw();
			menu.draw();
			window.display();

			continue;
		}


		sf::Time timer = clock.restart(); // sf::Time - to jest wynik // mierzy czas między klatkami
		float delta = timer.asSeconds();

		//Respawn bonusów
		obslugaRespawnBonusow(delta, respawn_time_limit, walls, speedup, timer_speedup, big_ball, timer_bigball, slowdown, timer_slowdown, pdouble, timer_pdouble);


		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
			if (event->is<sf::Event::Closed>())
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
		} // tu nic nie zmieniamy ^

		obslugaGracza1(gracz, bullets.data(), max_bullets, walls.data()); // dałem .data bo bez .data to nie działa bo to inny rodzaj tablicy
		obslugaGracza2(gracz2, bullets.data(), max_bullets, walls.data());

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
		obslugaKolizji(gracz, gracz2, bullets, walls, speedup, slowdown, big_ball, pdouble, max_bullets);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		for (int i = 0; i < walls.size(); i++) {
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

		//sprawdzanie konca gry
		if (gracz.health <= 0 || gracz2.health <= 0) {
			int winner = (gracz.health > 0) ? 1 : 2;

			menu.setEndScreen(winner); // Ustawia nagłówek i zmienia nazwy przycisków
			state = GameState::END_SCREEN;
		}

		window.display(); // wyświetla zawartość okna
	}
	return 0;
}