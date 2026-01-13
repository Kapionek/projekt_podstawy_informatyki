#include "obiekty.h"
#include <SFML/System/Angle.hpp>

void Object::_physics_process(float delta) { // ta funcja dzieje siê co klatkê i ka¿da struktura moze urzyæ tej funcji 
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);// która jest Object mo¿e mieæ ruch
	if (length != 0) { // aby na ukos nie biega³ szybciej 
		direction.x /= length;
		direction.y /= length;
		last_direction = direction;
	}
	if (movable) {
		velocity.y = direction.y * speed; // tu liczy siê prêdkoœæ
		velocity.x = direction.x * speed;
		sprite.move(velocity * delta);
		hard_accel = -5 * max_speed;
	}
	else {
		direction = { 0.f, 0.f }; // jeœli obiekt nie jest ruchomy to stoi w miejscu
		speed = 0;
	}
	if (!max_speed_on) {// tu jest zwalnianie normalne
		if (speed + used_accel * delta > 0.f) {
			speed += used_accel * delta;
		}
		else {
			speed = 0.f;
		}
	}
	if (dash_cooldown - delta >= 0.f) {  // tu obs³ugo cooldownu dasha
		dash_cooldown -= delta;
	}
	else {
		dash_cooldown = 0.f;
	}
	if (shot_cooldown - delta >= 0.f) {  // tu obs³ugo cooldownu dasha
		shot_cooldown -= delta;
	}
	else {
		shot_cooldown = 0.f;
	}
	if (speed > max_speed) { // tu to jest dla gracza na sztywno zrobione 
		used_accel = hard_accel; // to sprawia ¿e gracz szybko zwalnia do max prêdkoœci
	}			// przez co jest lepszy feal gry i dasha
	else {
		used_accel = accel;
	}  // tu dla normalnego accel
	if (team == 1) {
		sprite.setColor(sf::Color::Yellow);
	}
	if (team == 2) {
		sprite.setColor(sf::Color::Cyan);
	}
	if (damagable) {
		if (health <= 0) {
			visible = false;
		}
	}
	if (rotatable) {
		rotations();
	}
	if (buffable) {
		update_buffs(delta);
	}
}
void Object::update_buffs(float delta) {
	if (speed_buff_time - delta > 0.f) {
		speed_buff_time -= delta;
	}
	else {
		if (speedy) {
			remove_buffs(0);
		}
		speed_buff_time = 0.f;
	}
	if (slow_buff_time - delta > 0.f) {
		slow_buff_time -= delta;
	}
	else {
		if (slowy) {
			remove_buffs(1);
		}
		slow_buff_time = 0.f;
	}
	if (big_ball_time - delta > 0.f) {
		big_ball_time -= delta;
	}
	else {
		if (big_ball) {
			remove_buffs(2);
		}
		big_ball_time = 0.f;
	}
	if (multishot_time - delta > 0.f) {
		multishot_time -= delta;
	}
	else {
		if (multishot) {
			remove_buffs(3);
		}
		multishot_time = 0.f;
	}
}

void Object::remove_buffs(int rodzaj) {
	if (!buffable) {
		return;
	}
	switch (rodzaj) {
	case 0:
		speed_buff_time = 3.f;
		max_speed -= 100;
		speedy = false;
		break;
	case 1:
		slow_buff_time = 3.f;
		max_speed += 100;
		slowy = false;
		break;
	case 2:
		big_ball_time = 5.f;
		big_ball = false;
		break;
	case 3:
		multishot_time = 10.f;
		multishot = false;
		break;
	}
}

void Object::apply_buffs(int rodzaj) {
	if (!buffable) {
		return;
	}
	switch (rodzaj) {
	case 0:
		speed_buff_time = 3.f;
		max_speed += 100;
		speedy = true;
		break;
	case 1:
		slow_buff_time = 3.f;
		max_speed -= 100;
		slowy = true;
		break;
	case 2:
		big_ball_time = 5.f;
		big_ball = true;
		break;
	case 3:
		multishot_time = 10.f;
		multishot = true;
		break;
	}
}

void Object::rotations() {
	if (abs(direction.x) > 0.9f) {
		if (direction.x > 0) {
			sprite.setRotation(sf::degrees(0.f));
		}
		if (direction.x < 0) {
			sprite.setRotation(sf::degrees(180.f));
		}
	}
	else if (abs(direction.y) > 0.9f) {
		if (direction.y > 0) {
			sprite.setRotation(sf::degrees(90.f));
		}
		if (direction.y < 0) {
			sprite.setRotation(sf::degrees(270.f));
		}
	}
	else if (abs(direction.y) > 0.3 and abs(direction.x) > 0.3) {
		if (direction.y > 0) {
			if (direction.x > 0) {
				sprite.setRotation(sf::degrees(45.f));
			}
			if (direction.x < 0) {
				sprite.setRotation(sf::degrees(135.f));
			}
		}
		if (direction.y < 0) {
			if (direction.x > 0) {
				sprite.setRotation(sf::degrees(315.f));
			}
			if (direction.x < 0) {
				sprite.setRotation(sf::degrees(225.f));
			}
		}
	}
}

bool Object::sprawdzKolizje(const Object& inny) {
	if (!visible || !inny.visible) {
		return false; //sprawdzanie czy sprite'y siê dotykaj¹
	}
	if (sprite.getGlobalBounds().findIntersection(inny.sprite.getGlobalBounds())) {
		return true;
	}
	return false;
}