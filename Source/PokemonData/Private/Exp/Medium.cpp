﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp/Medium.h"

using namespace Exp;

REGISTER_SUBCLASS(IGrowthRate, FMedium);

int FMedium::ExpForLevel(int Level) const {
	if (Level <= 0)
		throw std::invalid_argument("A level value must be a positive integer!");

	if (Level == 1)
		return 0;

	return std::pow(Level, 3);
}
