// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "Parabolic.generated.h"

/**
 * Represents the Parabolic Exp. Growth Scheme
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UParabolic final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
