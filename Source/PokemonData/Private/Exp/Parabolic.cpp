// Created by Retro & Chill.


#include "Exp/Parabolic.h"

int UParabolic::ExpForLevel(int Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 6 / 5 - 15 * std::pow(Level, 2) + 100 * Level - 140;
}
