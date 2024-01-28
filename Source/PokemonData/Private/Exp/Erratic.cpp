// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp/Erratic.h"

int UErratic::ExpForLevel(int Level) const {
	if (Level <= 0)
		throw std::invalid_argument("A level value must be a positive integer!");

	if (Level == 1)
		return 0;

	if (Level <= 50)
		return std::pow(Level, 3) * (100 - Level) / 50;

	if (Level <= 68)
		return std::pow(Level, 3) * (150 - Level) / 100;

	if (Level <= 98)
		return std::pow(Level, 3) * ((1911 - 10 * Level) / 3) / 500;

	return std::pow(Level, 3) * (160 - Level) / 100;
}
