#include "zycie.h"

void Zycie::_physics_process(float delta) {
	if (visible) {
		Object::_physics_process(delta);  // tu by dzia³a³¹ fizyka z Object
	}
	if (id < gracz->health) {
		visible = true;
	}
	else {
		visible = false;
	}
}