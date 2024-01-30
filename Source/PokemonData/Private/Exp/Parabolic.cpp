// Created by Retro & Chill.


#include "Exp/Parabolic.h"

int UParabolic::ExpForLevel(int Level) const {
	if (Level <= 0)
		throw std::invalid_argument("A level value must be a positive integer!");

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 6 / 5 - 15 * std::pow(Level, 2) + 100 * Level - 140;
}
