#pragma once
#include <vector>
#include "obiekty.h"
#include "bullet.h"

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
);
