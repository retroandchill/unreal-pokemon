// Created by Retro & Chill.


#include "Exp/Fluctuating.h"

int UFluctuating::ExpForLevel(int Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	if (Level <= 15)
		return std::pow(Level, 3) * (24 + (Level + 1) / 3) / 50;

	if (Level <= 35)
		return std::pow(Level, 3) * (14 + Level) / 50;

	return std::pow(Level, 3) * (32 + Level / 2) / 50;
}
