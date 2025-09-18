// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Abilities/AbilityLookup.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/BlueprintClasses.h"

namespace Pokemon::Battle::Abilities
{
    TOptional<TNonNullSubclassOf<UGameplayAbility>> CreateAbilityEffect(FName ID)
    {
        return {}; // Classes::AbilityEffects.LoadClass(ID);
    }
} // namespace Pokemon::Battle::Abilities