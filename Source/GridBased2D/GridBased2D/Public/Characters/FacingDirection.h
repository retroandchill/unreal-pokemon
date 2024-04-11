// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "FacingDirection.generated.h"

/**
 * Enum to handle the facing direction of a character
 */
UENUM(BlueprintType)
enum class EFacingDirection : uint8 {
    /**
     * The character is facing down
     */
    Down,

    /**
     * The character is facing left
     */
    Left,

    /**
     * The character is facing right
     */
    Right,

    /**
     * The character is facing up
     */
    Up
};
