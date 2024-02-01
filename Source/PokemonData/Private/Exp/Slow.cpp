// Created by Retro & Chill.


#include "Exp/Slow.h"

int USlow::ExpForLevel(int Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3) * 5 / 4;
}
