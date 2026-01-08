#include "bullet.h"

void Bullet::_physics_process(float delta) {
	if (visible) {
		Object::_physics_process(delta);  // tu by dzia³a³¹ fizyka z Object
	}
	if (life_time - delta > 0) {  // obs³uga lifetime
		life_time -= delta;
	}
	else {
		life_time = 0;
	}
	if (life_time == 0) { // bullet po pewnym czasie znika
		visible = false;
	}
}
