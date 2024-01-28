// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrowthRate.h"
#include "Erratic.generated.h"

/**
 * Represents the Erratic Exp. Growth Scheme
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UErratic final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
