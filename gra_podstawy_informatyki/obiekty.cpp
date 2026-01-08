#include "obiekty.h"

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