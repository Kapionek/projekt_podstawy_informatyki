#pragma once
#include <SFML/Graphics.hpp>
#include "obiekty.h"
struct Bullet : public Object {  // tu zrobi³em strukture dziedzicz¹c¹ z Object dla bulletów tylko 
	float life_time = 4;
	virtual void _physics_process(float delta);
};