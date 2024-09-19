// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Templates/NonNullSubclassOf.h"

class UGameplayAbility;

namespace Pokemon::Battle::Abilities {
    /**
     * Lookup an ability effect by its ID
     * @param ID The ID that's associated with the ability effect
     * @return The instantiated gameplay ability for that ability (if an effect exists)
     */
    POKEMONBATTLE_API TOptional<TNonNullSubclassOf<UGameplayAbility>> CreateAbilityEffect(FName ID);
} // namespace Battle::Abilities