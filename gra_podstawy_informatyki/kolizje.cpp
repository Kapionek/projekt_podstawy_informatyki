#include "kolizje.h"
void obslugaKolizji(
	Object& gracz,
	Object& gracz2,
	std::vector<Bullet>& bullets,
	std::vector<Object>& walls,
	Object& speedup,
	Object& slowdown,
	Object& big_ball,
	Object& pdouble,
	int max_bullets
)
{
	for (int i = 0; i < max_bullets; i++) {
		if (bullets[i].visible) {
			for (int j = 0; j < walls.size(); j++) {
				//sprawdzanie, czy zasz³a kolizja i pobranie jej obszaru
				auto pole_kolizji = bullets[i].sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds());
				//zmienna przechowuje ma³y prostok¹t, czyli wspolny obszar pi³ki i œciany gdy zachodzi kolizja

				if (pole_kolizji) { //dosz³o do zderzenia
					if (pole_kolizji->size.x < pole_kolizji->size.y) { //uderzenie z boku (wê¿sze pole kolizji)
						bullets[i].direction.x *= -1.0f; // odwracanie kierunku poziomego
						if (bullets[i].sprite.getPosition().x > walls[j].sprite.getPosition().x) {
							bullets[i].sprite.move({ pole_kolizji->size.x, 0.0f }); // przesuwanie pi³ki poza œcianê
						}
						else {
							bullets[i].sprite.move({ -pole_kolizji->size.x, 0.0f });
						}
					}
					else {
						bullets[i].direction.y *= -1.0f;
						if (bullets[i].sprite.getPosition().y > walls[j].sprite.getPosition().y) {
							bullets[i].sprite.move({ pole_kolizji->size.y, 0.0f }); // przesuwanie pi³ki poza œcianê
						}
						else {
							bullets[i].sprite.move({ -pole_kolizji->size.y, 0.0f });
						}
					}
					bullets[i].sprite.move(bullets[i].direction * 2.0f);
				}
			}
			// kolizja z graczami

			if (bullets[i].sprawdzKolizje(gracz) and bullets[i].team != gracz.team) {
				gracz.health -= 1;
				bullets[i].visible = false;
			}
			if (bullets[i].sprawdzKolizje(gracz2) and bullets[i].team != gracz2.team) {
				gracz2.health -= 1;
				bullets[i].visible = false;
			}
		}
	}



	//œlizganie graczy na œcianach
	for (int j = 0; j < walls.size(); j++) {
		//kolizje dla 1 gracza
		auto pole_g1 = gracz.sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds());
		if (pole_g1) {
			if (pole_g1->size.x < pole_g1->size.y) {//kolizja pozioma
				float kierunek;
				if (gracz.sprite.getPosition().x < walls[j].sprite.getPosition().x) {
					kierunek = -1.0f; // wypchany w lewo, gdy¿ gracz jest po lewej stronie œciany
				}
				else {
					kierunek = 1.0f;
				}
				gracz.sprite.move({ pole_g1->size.x * kierunek, 0.0f });
				gracz.velocity.x = 0; //tylko bieg góra/dó³
				gracz.velocity.y *= 0.8f; //tarcie (spowolnienie)
			}
			else {//kolizja pionowa
				float kierunek;
				if (gracz.sprite.getPosition().y < walls[j].sprite.getPosition().y) {
					kierunek = -1.0f; // wypchany w górê, gdy¿ gracz jest nad œcian¹
				}
				else {
					kierunek = 1.0f;
				}
				gracz.sprite.move({ 0.0f, pole_g1->size.y * kierunek });
				gracz.velocity.y = 0; //tylko bieg prawo/lewo
				gracz.velocity.x *= 0.8f; //tarcie (spowolnienie)
			}
		}
		auto pole_g2 = gracz2.sprite.getGlobalBounds().findIntersection(walls[j].sprite.getGlobalBounds());
		if (pole_g2) {
			if (pole_g2->size.x < pole_g2->size.y) {//kolizja pozioma
				float kierunek;
				if (gracz2.sprite.getPosition().x < walls[j].sprite.getPosition().x) {
					kierunek = -1.0f; // wypchany w lewo, gdy¿ gracz jest po prawej stronie œciany
				}
				else {
					kierunek = 1.0f;
				}
				gracz2.sprite.move({ pole_g2->size.x * kierunek, 0.0f });
				gracz2.velocity.x = 0; //tylko bieg góra/dó³
				gracz2.velocity.y *= 0.8f; //tarcie (spowolnienie)
			}
			else {//kolizja pionowa
				float kierunek;
				if (gracz2.sprite.getPosition().y < walls[j].sprite.getPosition().y) {
					kierunek = -1.0f; // wypchany w górê, gdy¿ gracz jest nad œcian¹
				}
				else {
					kierunek = 1.0f;
				}
				gracz2.sprite.move({ 0.0f, pole_g2->size.y * kierunek });
				gracz2.velocity.y = 0; //tylko bieg prawo/lewo
				gracz2.velocity.x *= 0.8f; //tarcie (spowolnienie)
			}
		}
	}
	//kolizja miêdzy 1 a 2 graczem
	//sprawdzanie czy gracze na siebie nachodz¹, jeœli tak to tworzy siê ich czêœæ wspólna
	auto pole_zderzenia = gracz.sprite.getGlobalBounds().findIntersection(gracz2.sprite.getGlobalBounds());
	if (pole_zderzenia) {//gracze siê zderzyli
		if (pole_zderzenia->size.x < pole_zderzenia->size.y) {//jeœli prostok¹t kolizji jest wê¿szy ni¿ wy¿szy to zderzyli siê poziomo
			//kolizja pozioma
			float kierunek;
			if (gracz.sprite.getPosition().x < gracz2.sprite.getPosition().x) {
				kierunek = -1.0f; //gracz 1 jest po lewej, wiêc wypycha w lewo
			}
			else {
				kierunek = 1.0f; //gracz 1 jest z prawej wiêc wypycha w prawo
			}
			gracz.sprite.move({ (pole_zderzenia->size.x / 2.0f) * kierunek,0.0f }); //g³êbokoœæ zderzenie dzielona na pó³ i przesuniêcie obu w przeciwnych kierunkach
			gracz2.sprite.move({ (pole_zderzenia->size.x / 2.0f) * -kierunek,0.0f });

			gracz.velocity.x = 0;//zerowanie prêdkoœci w osi poziomej czyli tej w której jest zderzenie w tym przypadku, prêdkoœæ y bez zmian
			gracz2.velocity.x = 0;
		}
		else {
			//kolizja pionowa
			float kierunek;
			if (gracz.sprite.getPosition().y < gracz2.sprite.getPosition().y) {
				kierunek = -1.0f; //gracz 1 jest nad graczem 2, wiêc wypycha w górê
			}
			else {
				kierunek = 1.0f; //gracz 1 jest pod graczem 2, wiêc wypycha w dó³
			}
			gracz.sprite.move({ 0.0f, (pole_zderzenia->size.y / 2.0f) * kierunek });
			gracz2.sprite.move({ 0.0f, (pole_zderzenia->size.y / 2.0f) * -kierunek });

			gracz.velocity.y = 0;
			gracz2.velocity.y = 0;
		}
	}
	if (speedup.visible) {
		//gracz1 z przyœpieszeniem
		if (gracz.sprawdzKolizje(speedup)) {
			speedup.visible = false;
			gracz.apply_buffs(0);
		}
		//gracz2 z przyœpieszeniem
		if (gracz2.sprawdzKolizje(speedup)) {
			speedup.visible = false;
			gracz2.apply_buffs(0);
		}
	}
	if (slowdown.visible) {
		//gracz1 ze spowolnieniem
		if (gracz.sprawdzKolizje(slowdown)) {
			gracz2.apply_buffs(1);
			slowdown.visible = false;
		}
		//gracz2 ze spowolnieniem
		if (gracz2.sprawdzKolizje(slowdown)) {
			gracz.apply_buffs(1);
			slowdown.visible = false;
		}
	}
	if (big_ball.visible) {
		//gracz1 z powiêkszniem kulki
		if (gracz.sprawdzKolizje(big_ball)) {
			gracz.apply_buffs(2);
			big_ball.visible = false;
		}
		//gracz2 z powiêkszniem kulki
		if (gracz2.sprawdzKolizje(big_ball)) {
			gracz2.apply_buffs(2);
			big_ball.visible = false;
		}
	}
	if (pdouble.visible) {
		//gracz 1 z podwójnym strza³em
		if (gracz.sprawdzKolizje(pdouble)) {
			gracz.apply_buffs(3);
			pdouble.visible = false;
		}
		//gracz 2 z podwójnym strza³em
		if (gracz2.sprawdzKolizje(pdouble)) {
			gracz2.apply_buffs(3);
			pdouble.visible = false;
		}
	}

}