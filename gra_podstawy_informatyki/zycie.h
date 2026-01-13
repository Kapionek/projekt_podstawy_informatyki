#pragma once
#include <SFML/Graphics.hpp> 
#include "obiekty.h"
struct Zycie : public Object {
	Object* gracz = nullptr;
	int id =  0;
	Zycie(const sf::Texture& texture) : Object(texture) {}

	virtual void _physics_process(float delta);
};