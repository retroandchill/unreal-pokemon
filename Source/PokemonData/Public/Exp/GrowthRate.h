// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.generated.h"


// This class does not need to be modified.
UINTERFACE(Blueprintable)
class UGrowthRate : public UInterface {
	GENERATED_BODY()
};


/**
 * Interface to represent various Exp. grow rates
 */
class POKEMONDATA_API IGrowthRate {
	GENERATED_BODY()

public:
	/**
	 * Gets the amount of Exp that is required to reach a certain level 
	 * @param Level The level in to calculate the Exp. for
	 * @return The amount of Exp. required to level up
	 */
	UFUNCTION()
	virtual int ExpForLevel(int Level) const = 0;
};
