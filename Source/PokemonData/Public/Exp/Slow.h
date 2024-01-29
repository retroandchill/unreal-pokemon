// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "Slow.generated.h"

/**
 * Represents the Slow Exp. Growth Scheme
 */
UCLASS(BlueprintType)
class POKEMONDATA_API USlow final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
