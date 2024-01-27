// Created by Retro & Chill.


#include "Exp/Slow.h"

using namespace Exp;

REGISTER_SUBCLASS(GrowthRate, Slow);

int Slow::ExpForLevel(int Level) const {
	if (Level <= 0)
		throw std::invalid_argument("A level value must be a positive integer!");

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 5 / 4;
}
