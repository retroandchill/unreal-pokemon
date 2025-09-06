// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Represents the possible genders of a trainer
 */
UENUM(BlueprintType)
enum class ETrainerGender : uint8
{
    /**
     * Represents a male trainer
     */
    Male,

    /**
     * Represents a female trainer
     */
    Female,

    /**
     * Represents a trainer of unknown gender
     */
    Unknown,

    /**
     * Represents a double battle trainer with a male and female member
     */
    Mixed
};
