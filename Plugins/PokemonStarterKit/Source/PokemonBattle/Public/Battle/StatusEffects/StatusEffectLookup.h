// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Optional/OptionalRef.h"

struct FStatus;
class UGameplayEffect;

namespace Pokemon::Battle::StatusEffects {

    /**
     * Lookup a status effect by its ID
     * @param ID The ID that's associated with the hold item effect
     * @return The gameplay ability class for that item (if an effect exists)
     */
    POKEMONBATTLE_API TOptional<TNonNullSubclassOf<UGameplayEffect>> FindStatusEffect(FName ID);

    /**
     * Lookup a status effect by its ID
     * @param Status The status effect to look up
     * @return The gameplay effect class for that status effect (if an effect exists)
     */
    POKEMONBATTLE_API TOptional<TNonNullSubclassOf<UGameplayEffect>>
    FindStatusEffect(TOptional<const FStatus &> Status);
} // namespace Pokemon::Battle::StatusEffects