﻿// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"
#include "UObject/Object.h"

#include "BattleUse.generated.h"

/**
 * Represents the different options for how an item can be used in battle
 */
UENUM(BlueprintType)
enum class EBattleUse : uint8 {
    /**
     * Not usable in battle
     */
    NoBattleUse = 0,

    /**
     * Usable on a Pokémon in the party
     */
    OnPokemon = 1,

    /**
     * Usable on a Pokémon in the party and requiring a move to be selected
     */
    OnMove = 2,

    /**
     * Usable on the active Pokémon in battle
     */
    OnBattler = 3,

    /**
     * Used on an opponent in battle
     */
    OnFoe = 4,

    /**
     * Used directly with no target selection
     */
    Direct = 5
};

namespace Pokemon::Data::Bag {

    /**
     * Tag for items that restore HP.
     */
    POKEMONDATA_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(HPPPRestore);

    /**
     * Tag for items that cure status effects.
     */
    POKEMONDATA_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(StatusRestore);

    /**
     * Tag for items that are Poké Balls
     */
    POKEMONDATA_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(PokeBall);

    /**
     * Tag for items that are considered Battle Usage.
     */
    POKEMONDATA_API const UE_DECLARE_GAMEPLAY_TAG_EXTERN(BattleUse);

} // namespace Pokemon::Data::Bag