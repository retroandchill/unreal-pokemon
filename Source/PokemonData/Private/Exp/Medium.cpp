// Fill out your copyright notice in the Description page of Project Settings.


#include "Exp/Medium.h"

FName UMedium::GetId() const {
	return "Medium";
}

int UMedium::ExpForLevel(int Level) const {
	return std::pow(Level, 3);
}
