// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp/Medium.h"

using namespace Exp;

REGISTER_SUBCLASS(GrowthRate, Medium);

FName Medium::GetId() const {
	return "Medium";
}

int Medium::ExpForLevel(int Level) const {
	return std::pow(Level, 3);
}
