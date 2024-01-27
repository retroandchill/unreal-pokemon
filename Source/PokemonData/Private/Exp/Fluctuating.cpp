// Created by Retro & Chill.


#include "Exp/Fluctuating.h"

using namespace Exp;


REGISTER_SUBCLASS(GrowthRate, Fluctuating);

int Fluctuating::ExpForLevel(int Level) const {
	if (Level <= 0)
		throw std::invalid_argument("A level value must be a positive integer!");
	
	if (Level == 1)
		return 0;

	if (Level <= 15)
		return std::pow(Level, 3) * (24 + (Level + 1) / 3) / 50;

	if (Level <= 35)
		return std::pow(Level, 3) * (14 + Level) / 50;

	return std::pow(Level, 3) * (32 + Level / 2) / 50;
}
