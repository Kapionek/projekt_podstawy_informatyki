#include "sterowanie.h"
void obslugaGracza1(Object& gracz, Bullet bullets[], int max_bullets, Object walls[]) {
	if (gracz.speed <= 0.f) { // ta czêœæ obs³uguje gracza i te¿ zwalnianie
		gracz.direction = { 0.f, 0.f };
	}
	gracz.max_speed_on = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //tu jest ogarniani unputu z klawiatury
		if (gracz.speed <= gracz.max_speed) {  // ten warunek sprawia ¿e nie oddzia³owywuje gdy dashujesz przez co nie mo¿esz zminiaæ kierunku dasha
			gracz.direction.y = -1; //to jest specjalnie bo wspó³rzêdne y-kowe s¹ na odwrót. x -owe s¹ normalnie jak coœ 
			gracz.max_speed_on = true; // dla 4 inputów (WSAD)
			gracz.speed = gracz.max_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
		if (gracz.speed <= gracz.max_speed) {
			gracz.direction.x = 1;
			gracz.max_speed_on = true;
			gracz.speed = gracz.max_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {  // tu jest dash 
		if (gracz.dash_cooldown <= 0.f) {
			gracz.speed = 3.f* gracz.max_speed;
			gracz.dash_cooldown = 0.5;
			gracz.direction = gracz.last_direction; // u¿ywa last direction by mo¿na by³o dashowaæ jak sie stoi
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {  //strza³ pi³k¹ 
		if (gracz.shot_cooldown <= 0.f) {
			int ile_pociskow;
			if (gracz.multishot == true) {
				ile_pociskow = 2; //z bonusme strza³ 2 razy
			}
			else {
				ile_pociskow = 1;
			}
			int wystrzelone = 0;
			for (int i = 0; i < max_bullets; i++) {	 // tu przechodzi przez wszystkie dostêpne bullety dostêpne 
				if (bullets[i].visible == false) {
					bullets[i].visible = true;  // tu zmienia parametry i ustawia dla tego bulleta 
					if (!gracz.big_ball) {
						bullets[i].sprite.setPosition(gracz.sprite.getPosition());
					}
					else {
						bullets[i].sprite.setPosition(gracz.sprite.getPosition() - gracz.last_direction * 8.f);
					}
					bullets[i].direction = gracz.last_direction;
					bullets[i].speed = 500.f;
					bullets[i].normal_speed = 500.f;
					bullets[i].life_time = 10.f;
					bullets[i].team = gracz.team;
					//ustalnienie kierunku
					if (wystrzelone == 0) {
						bullets[i].direction = gracz.last_direction;
					}
					else {
						bullets[i].direction = -gracz.last_direction;
					}
					//Powiêksznie pi³ki
					if (gracz.big_ball) {
						bullets[i].sprite.setScale({ 2.0f,2.0f });
					}
					else {
						bullets[i].sprite.setScale({ 1.0f, 1.0f });
					}
					wystrzelone = wystrzelone + 1;
					if (wystrzelone >= ile_pociskow) {
						break;
					}
					
				}
			}
			gracz.shot_cooldown = 2;
		}
	}
}
void obslugaGracza2(Object& gracz2, Bullet bullets[], int max_bullets, Object walls[]) {
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) {
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
		if (gracz2.speed <= gracz2.max_speed) {
			gracz2.direction.x = 1;
			gracz2.max_speed_on = true;
			gracz2.speed = gracz2.max_speed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) {
		if (gracz2.dash_cooldown <= 0.f) {
			gracz2.speed = 3.f * gracz2.max_speed;
			gracz2.dash_cooldown = 0.5;
			gracz2.direction = gracz2.last_direction;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) {
		if (gracz2.shot_cooldown <= 0.f) {
			int ile_pociskow;
			if (gracz2.multishot == true) {
				ile_pociskow = 2; //z bonusme strza³ 2 razy
			}
			else {
				ile_pociskow = 1;
			}
			int wystrzelone = 0;
			for (int i = 0; i < max_bullets; i++) {
				if (bullets[i].visible == false) {
					bullets[i].visible = true;
					if (!gracz2.big_ball) {
						bullets[i].sprite.setPosition(gracz2.sprite.getPosition());
					}
					else {
						bullets[i].sprite.setPosition(gracz2.sprite.getPosition() - gracz2.last_direction * 8.f);
					}
					bullets[i].direction = gracz2.last_direction;
					bullets[i].speed = 500.f;
					bullets[i].normal_speed = 500.f;
					bullets[i].life_time = 10.f;
					bullets[i].team = gracz2.team;
					bullets[i].team = gracz2.team;
					//ustalnienie kierunku
					if (wystrzelone == 0) {
						bullets[i].direction = gracz2.last_direction;
					}
					else {
						bullets[i].direction = -gracz2.last_direction;
					}
					//Powiêksznie pi³ki
					if (gracz2.big_ball) {
						bullets[i].sprite.setScale({ 2.0f,2.0f });
					}
					else {
						bullets[i].sprite.setScale({ 1.0f, 1.0f });
					}
					wystrzelone = wystrzelone + 1;

					if (wystrzelone >= ile_pociskow) {
						break;
					}
				}
			}
			gracz2.shot_cooldown = 2;
		}
	}
}

