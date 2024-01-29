// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "Fast.generated.h"

/**
 * Represents the Fast Exp. Growth Scheme
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UFast final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
