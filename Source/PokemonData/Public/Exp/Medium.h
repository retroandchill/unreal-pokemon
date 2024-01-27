// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrowthRate.h"
#include "UObject/Object.h"
#include "Medium.generated.h"

/**
 * Medium/Medium Fast Exp Growth curve
 */
UCLASS(BlueprintType)
class POKEMONDATA_API UMedium : public UObject, IGrowthRate {
	GENERATED_BODY()

public:
	virtual FName GetId() const override;
	virtual int ExpForLevel(int Level) const override;
};
