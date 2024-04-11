// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"

/**
 * The different target options a move can have
 */
UENUM(BlueprintType)
enum class ETargetCount : uint8 {
    /**
     * The move has no explicit target. This means the target is either the user or the move has some other special
     * property when it comes to its target.
     */
    NoneOrSelf = 0,

    /**
     * The move has a single explicit target that the player selects when using the move
     */
    SingleTarget = 1,

    /**
     * The move has multiple possible targets, and thus the move will select all availible targets
     */
    MultiTarget = 2
};
