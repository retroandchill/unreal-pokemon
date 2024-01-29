// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GrowthRate.h"
#include "Medium.generated.h"


/**
 * Medium/Medium Fast Exp Growth curve
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UMedium final : public UObject, public IGrowthRate {
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual int ExpForLevel(int Level) const override;
};
