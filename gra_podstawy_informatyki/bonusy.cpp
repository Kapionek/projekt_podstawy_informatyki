#include "bonusy.h"
#include <cstdlib>

void obslugaRespawnBonusow(
	float delta,
	float respawn_time_limit,
	std::vector<Object>& walls,
	Object& speedup,
	float& timer_speedup,
	Object& big_ball,
	float& timer_bigball,
	Object& slowdown,
	float& timer_slowdown,
	Object& pdouble,
	float& timer_pdouble
) {
	//sprawdzanie ka¿dego bonusu
	if (speedup.visible == false) {
		timer_speedup += delta;
		if (timer_speedup >= respawn_time_limit) {
			bool kolizja = true;
			while (kolizja) {
				speedup.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				kolizja = false; //zak³adam ¿e jest ok
				speedup.visible = true;
				//sprawdzanie czy dotyka œciany
				for (int i = 0; i < walls.size(); i++) {
					if (speedup.sprawdzKolizje(walls[i])) {
						kolizja = true; //trafi³o na œcianê
						break;
					}
				}
			}
			timer_speedup = 0.f;
		}
	}
	if (big_ball.visible == false) {
		timer_bigball += delta;
		if (timer_bigball >= respawn_time_limit) {
			bool kolizja = true;
			while (kolizja) {
				big_ball.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				kolizja = false; //zak³adam ¿e jest ok
				big_ball.visible = true;
				//sprawdzanie czy dotyka œciany
				for (int i = 0; i < walls.size(); i++) {
					if (big_ball.sprawdzKolizje(walls[i])) {
						kolizja = true; //trafi³o na œcianê
						break;
					}
				}
			}
			timer_bigball = 0.f;
		}
	}
	if (slowdown.visible == false) {
		timer_slowdown += delta;
		if (timer_slowdown >= respawn_time_limit) {
			bool kolizja = true;
			while (kolizja) {
				slowdown.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				kolizja = false; //zak³adam ¿e jest ok
				slowdown.visible = true;
				//sprawdzanie czy dotyka œciany
				for (int i = 0; i < walls.size(); i++) {
					if (slowdown.sprawdzKolizje(walls[i])) {
						kolizja = true; //trafi³o na œcianê
						break;
					}
				}
			}
			timer_slowdown = 0.f;
		}
	}
	if (pdouble.visible == false) {
		timer_pdouble += delta;
		if (timer_pdouble >= respawn_time_limit) {
			bool kolizja = true;
			while (kolizja) {
				pdouble.sprite.setPosition({ (float)(rand() % 600 + 100), (float)(rand() % 600 + 100) });
				kolizja = false; //zak³adam ¿e jest ok
				pdouble.visible = true;
				//sprawdzanie czy dotyka œciany
				for (int i = 0; i < walls.size(); i++) {
					if (pdouble.sprawdzKolizje(walls[i])) {
						kolizja = true; //trafi³o na œcianê
						break;
					}
				}
			}
			timer_pdouble = 0.f;
		}
	}
}