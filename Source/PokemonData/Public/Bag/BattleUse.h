// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleUse.generated.h"

/**
 * Represents the different options for how an item can be used in battle
 */
UENUM(BlueprintType)
enum class EBattleUse : uint8
{
 NoBattleUse = 0,
 OnPokemon = 1,
 OnMove = 2,
 OnBattler = 3,
 OnFoe = 4,
 Direct = 5
};
