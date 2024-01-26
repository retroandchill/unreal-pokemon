// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FieldUse.generated.h"

/**
 * Represents the different options for how an item can be used in the field
 */
UENUM()
enum class EFieldUse : uint8
{
	NoFieldUse = 0,
	OnPokemon = 1,
	Direct = 2,
	TM = 3,
	HM = 4,
	TR = 5
};
