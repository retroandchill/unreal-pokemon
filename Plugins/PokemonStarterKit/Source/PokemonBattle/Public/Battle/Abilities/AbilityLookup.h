// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

class UGameplayAbility;

namespace Battle::Abilities {
    /**
     * Lookup an ability effect by its ID
     * @param ID The ID that's associated with the ability effect
     * @return The instantiated gameplay ability for that ability (if an effect exists)
     */
    POKEMONBATTLE_API TSubclassOf<UGameplayAbility> CreateAbilityEffect(FName ID);
} // namespace Battle::Abilities