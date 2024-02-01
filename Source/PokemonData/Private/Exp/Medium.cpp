// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp/Medium.h"

int UMedium::ExpForLevel(int Level) const {
	check(Level > 0);

	if (Level == 1)
		return 0;

	return std::pow(Level, 3);
}
