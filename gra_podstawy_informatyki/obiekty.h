#pragma once
#include <SFML/Graphics.hpp> 

struct Object { //podstawowa struktura
	sf::CircleShape sprite;
	sf::Vector2f direction;
	sf::Vector2f last_direction; // to siê przyda potem 
	sf::Vector2f velocity;
	sf::Color color;
	float speed = 0.f;
	float max_speed = 150.f;
	float accel = -250.f;
	float hard_accel = -500.f;  // to jest accel do dasha
	float used_accel = -250.f;  // aktualny accel
	float dash_cooldown = 0.f;
	float shot_cooldown = 0.f;
	bool movable = true;
	bool max_speed_on = false;
	bool visible = true;
	virtual void _physics_process(float delta);
};
