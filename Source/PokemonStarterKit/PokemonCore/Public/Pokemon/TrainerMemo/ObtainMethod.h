// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "ObtainMethod.generated.h"

/**
 * The enum used to determine the method the Pokémon was obtained as
 */
UENUM(BlueprintType)
enum class EObtainMethod : uint8
{
    Default,
    Egg,
    Trade,
    FatefulEncounter
};