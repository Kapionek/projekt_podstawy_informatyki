#pragma once
#include <SFML/Graphics.hpp> 
#include <vector>

struct Object { //podstawowa struktura
	sf::Sprite sprite;
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
	bool rotatable = false;
	bool movable = true;
	bool max_speed_on = false;
	bool visible = true;
	bool damagable = false;
	bool buffable = false;
	bool speedy = false;
	bool slowy = false;
	bool big_ball = false;
	bool multishot = false;
	int health = 3;
	int team = 0; // team 0 to jest bazowy team a teamy 1 i 2 to s¹ teamy graczy jak coœ 
	float speed_buff_time = 0.f;
	float slow_buff_time = 0.f;
	float big_ball_time = 0.f;
	float multishot_time = 0.f;
	float normal_speed = 150.f;

	Object(const sf::Texture& texture) : sprite(texture) {}  // to sprawia ¿e potem mo¿esz napisaæ Object(texture) i to daje texture
															 // jako texture sprita poprzez sprite(texture) 
	virtual void _physics_process(float delta);

	void rotations();

	void apply_buffs(int rodzaj);
	void update_buffs(float delta);
	void remove_buffs(int rodzaj);

	bool sprawdzKolizje(const Object& inny);
};
