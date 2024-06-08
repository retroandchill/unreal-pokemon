// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "FieldUse.generated.h"

/**
 * Represents the different options for how an item can be used in the field
 */
UENUM(BlueprintType)
enum class EFieldUse : uint8 {
    /**
     * Not usable in the field
     */
    NoFieldUse = 0,

    /**
     * Used on a Pokémon
     */
    OnPokemon = 1,

    /**
     * Used directly from the bag
     */
    Direct = 2,

    /**
     * Teaches a Pokémon a move (resuable on newer mechanics)
     */
    TM = 3,

    /**
     * Teaches a Pokémon a move (reusable)
     */
    HM = 4,

    /**
     * Teaches a Pokémon a move (single-use)
     */
    TR = 5
};
