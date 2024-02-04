// Created by Retro & Chill.


#include "Exp/Fast.h"

int UFast::ExpForLevel(int Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 4 / 5;
}
