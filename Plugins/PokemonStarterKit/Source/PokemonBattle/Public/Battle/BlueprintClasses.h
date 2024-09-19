// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Assets/AssetClass.h"
#include "Items/BattleItemEffect.h"

namespace Pokemon::Battle::Classes {
    /**
     * Blueprint class lookup for move effects.
     */
    UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(POKEMONBATTLE_API, MoveEffects, UBattleMoveFunctionCode);

    /**
     * Blueprint class lookup for item usage effects.
     */
    UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(POKEMONBATTLE_API, ItemEffects, UBattleItemEffect);

    /**
     * Blueprint class lookup for ability effects.
     */
    UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(POKEMONBATTLE_API, AbilityEffects, UGameplayAbility);
    
    /**
     * Blueprint class lookup for hold item effects.
     */
    UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(POKEMONBATTLE_API, HoldItemEffects, UGameplayAbility);

    /**
     * Blueprint class lookup for status effects.
     */
    UE_DECLARE_BLUEPRINT_CLASS_EXPORTED(POKEMONBATTLE_API, StatusEffects, UGameplayEffect);
}