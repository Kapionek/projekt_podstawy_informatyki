#pragma once
#include <vector>
#include "obiekty.h"

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
);