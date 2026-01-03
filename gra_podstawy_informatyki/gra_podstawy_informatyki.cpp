#include <SFML/Graphics.hpp>  // biblioteka SFML

struct Object { //podstawowa struktura
	sf::CircleShape sprite; 
	sf::Vector2f direction;
	sf::Vector2f last_direction; // to się przyda potem 
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

	virtual void _physics_process(float delta) { // ta funcja dzieje się co klatkę i każda struktura moze urzyć tej funcji 
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);// która jest Object może mieć ruch
		if (length != 0) { // aby na ukos nie biegał szybciej 
			direction.x /= length;
			direction.y /= length;
			last_direction = direction;
		}
		if (movable) {
			velocity.y = direction.y * speed; // tu liczy się prędkość
			velocity.x = direction.x * speed;
			sprite.move(velocity * delta);
		}
		else {
			direction = { 0.f, 0.f }; // jeśli obiekt nie jest ruchomy to stoi w miejscu
			speed = 0;
		}
		if (!max_speed_on) {  // tu jest zwalnianie normalne
			if (speed + used_accel * delta> 0.f) {
				speed += used_accel * delta;
			}
			else {
				speed = 0.f;
			}
		}
		if (dash_cooldown - delta >= 0.f) {  // tu obsługo cooldownu dasha	
			dash_cooldown -= delta;
		}
		else {
			dash_cooldown = 0.f;
		}
		if (shot_cooldown - delta >= 0.f) {  // tu obsługo cooldownu dasha
			shot_cooldown -= delta;
		}
		else {
			shot_cooldown = 0.f;
		}
		if (speed > max_speed) { // tu to jest dla gracza na sztywno zrobione 
			used_accel = hard_accel; // to sprawia że gracz szybko zwalnia do max prędkości
		}			// przez co jest lepszy feal gry i dasha
		else {
			used_accel = accel;
		}  // tu dla normalnego accel
	}
};

struct Bullet : public Object {  // tu zrobiłem strukture dziedziczącą z Object dla bulletów tylko 
	float life_time = 4;
	virtual void _physics_process(float delta) {  
		if (visible) {
			Object::_physics_process(delta);  // tu by działałą fizyka z Object
		}
		if (life_time - delta > 0) {  // obsługa lifetime
			life_time -= delta;
		}
		else {
			life_time = 0;
		}
		if (life_time == 0) { // bullet po pewnym czasie znika
			visible = false;
		}
	}
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka
	const int max_bullets = 10; // const jest potrzebny do tablicy  

	// inicjacja obiektów

	Bullet bullets[10]; // tablica na pociski
	for (int i = 0; i < max_bullets; i++) {
		bullets[i].visible = false;
		bullets[i].sprite = sf::CircleShape(3.f);
		bullets[i].color = sf::Color::Yellow;
		bullets[i].sprite.setFillColor(sf::Color::Yellow);
		bullets[i].accel = -50.f;
	}

	Object gracz; // tu nazywasz obiekt i piszesz co to za obiekt ( narazie wszystko z Object bo będzie najłatwiej )
	gracz.color = sf::Color::Cyan;  // tu zmieniasz wartości tych zmiennych z góry z struktury
	gracz.sprite = sf::CircleShape(10.f);
	gracz.sprite.setFillColor(gracz.color);
	gracz.sprite.setPosition({ 100.f, 100.f });
	gracz.accel = -250.f;

	sf::Clock clock; // zegar do mierzenia czasu między klatkami
	while (window.isOpen()) {   // to spawia że gra działa dopóki okno jest otwarte
		sf::Time dt = clock.restart(); // mierzy czas między klatkami
        float delta = dt.asSeconds(); 

		while (const std::optional event = window.pollEvent()) { // sparawdza czy zaszło jakieś zdarzenie (np. kliknięcie krzyżyka)
            if (event->is<sf::Event::Closed>()) 
				window.close(); // zamyka okno kiedy klikniemy na krzyżyk
        } // tu nic nie zmieniamy ^

		if (gracz.speed <= 0.f) { // ta część obsługuje gracza i też zwalnianie
			gracz.direction = { 0.f, 0.f };
		}
		gracz.max_speed_on = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { //tu jest ogarniani unputu z klawiatury
			if (gracz.speed <= gracz.max_speed) {  // ten warunek sprawia że nie oddziałowywuje gdy dashujesz przez co nie możesz zminiać kierunku dasha
				gracz.direction.y = -1; //to jest specjalnie bo współrzędne y-kowe są na odwrót. x -owe są normalnie jak coś 
				gracz.max_speed_on = true; // dla 4 inputów (WSAD)
				gracz.speed = gracz.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
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
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			if (gracz.speed <= gracz.max_speed) {
				gracz.direction.x = 1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) {  // tu jest dash 
			if (gracz.dash_cooldown <= 0.f) {
				gracz.speed = 350.f;
				gracz.dash_cooldown = 0.5;
				gracz.direction = gracz.last_direction; // używa last direction by można było dashować jak sie stoi
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {  //strzał piłką 
			if (gracz.shot_cooldown <= 0.f) { 
				for (int i = 0; i < max_bullets; i++) {	 // tu przechodzi przez wszystkie dostępne bullety dostępne 
					if (bullets[i].visible == false) {  
						bullets[i].visible = true;  // tu zmienia parametry i ustawia dla tego bulleta 
						bullets[i].sprite.setPosition(gracz.sprite.getPosition());
						bullets[i].direction = gracz.last_direction;
						bullets[i].speed = 400.f;
						bullets[i].life_time = 3.f;
						break; // by nie szukał dalej
					}
				}
				gracz.shot_cooldown = 2;
			}
		}
		// tu _physic_process()
		gracz._physics_process(delta);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		for (int i = 0; i < max_bullets; i++) {
			if (bullets[i].visible) {
				bullets[i]._physics_process(delta);
				window.draw(bullets[i].sprite);
			}
		}
		if (gracz.visible) {
			window.draw(gracz.sprite);
		}
		window.display(); // wyświetla zawartość okna
    }
    return 0;
}