#pragma once
#include <SFML/Graphics.hpp>
#include "obiekty.h"
struct Bullet : public Object {  // tu zrobi³em strukture dziedzicz¹c¹ z Object dla bulletów tylko 
	float life_time = 4;

	Bullet(const sf::Texture& texture) : Object(texture) {}

	virtual void _physics_process(float delta);
};