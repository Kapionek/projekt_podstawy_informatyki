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
	float hard_accel = -500.f;
	float used_accel = -250.f;
	float dash_cooldown = 0.f;
	bool movable = true;
	bool max_speed_on = false;
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
		if (speed > max_speed) { // tu to jest dla gracza na sztywno zrobione 
			used_accel = hard_accel; // to sprawia że gracz szybko zwalnia do max prędkości
		}			// przez co jest lepszy feal gry i dasha
		else {
			used_accel = accel;
		}  // tu dla normalnego accel
	}
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 800,800 }), "Zbijak");  // tworzy okno w {} jest rozmiar, "Zbijak" wyświetla się na górze okienka

	//inicjacja obiektów 
	Object pilka;  // tu nazywasz obiekt i piszesz co to za obiekt ( narazie wszystko z Object bo będzie najłatwiej )
	pilka.color = sf::Color::Red;  // tu zmieniasz wartości tych zmiennych z góry z struktury  
	pilka.sprite = sf::CircleShape(5.f);
	pilka.sprite.setFillColor(pilka.color);
	pilka.sprite.setPosition({400.f, 400.f});
	pilka.direction = { 1.f, 1.f };
	pilka.speed = 250;
	pilka.max_speed = 250.f;

	Object pilka2;
	pilka2.color = sf::Color::Blue;
	pilka2.sprite = sf::CircleShape(5.f);
	pilka2.sprite.setFillColor(pilka2.color);
	pilka2.sprite.setPosition({200.f, 200.f});
	pilka2.direction = { -1.f, 1.f };
	pilka2.speed = 250;
	pilka2.max_speed = 250.f;

	Object gracz;
	gracz.color = sf::Color::Cyan;
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
			if (gracz.speed < gracz.max_speed) {  // ten warunek sprawia że nie oddziałowywuje gdy dashujesz przez co nie możesz zminiać kierunku dasha
				gracz.direction.y = -1;//to jest specjalnie bo współrzędne y-kowe są na odwrót. x -owe są normalnie jak coś 
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { // dla 4 inputów (WSAD
			if (gracz.speed < gracz.max_speed) {
				gracz.direction.y = 1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			if (gracz.speed < gracz.max_speed) {
				gracz.direction.x = -1;
				gracz.max_speed_on = true;
				gracz.speed = gracz.max_speed;
			}
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			if (gracz.speed < gracz.max_speed) {
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
		// tu _physic_process()
		pilka._physics_process(delta); 
		pilka2._physics_process(delta);
		gracz._physics_process(delta);
		// tu pomiędzy clear() a display() dajemy draw do obiektów bo bez draw nic sie nie wyświetli
		window.clear();  // czyści okno z poprzedniej klatki
		window.draw(pilka.sprite);
		window.draw(pilka2.sprite);
		window.draw(gracz.sprite);
		window.display(); // wyświetla zawartość okna
    }
    return 0;
}