// Created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "Fluctuating.generated.h"

/**
* Represents the Fluctuating Exp. Growth Scheme
*/
UCLASS(BlueprintType)
class POKEMONDATA_API UFluctuating final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
