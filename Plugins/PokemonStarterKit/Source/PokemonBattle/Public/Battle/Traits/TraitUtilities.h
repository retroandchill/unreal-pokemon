// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "IndividualTraitHolder.h"
#include "Algo/ForEach.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Damage/DamageModificationTrait.h"
#include "Damage/DamageModificationTraits.h"
#include "RangeHelpers.h"

/**
 * Apply the damage modification traits with the given trait information
 * @param Type The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 * @param TraitHolder The trait holder object to process 
 */
#define APPLY_DAMAGE_MODIFICATION(Type, Multipliers, Context, TraitHolder) \
    Traits::ApplyDamageModificationTraits<&FIndividualDamageModifierTraits::Type>(Multipliers, Context, TraitHolder)

/**
 * Shorthand to create a macro for iterating over trait holders
 * @param Type The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
#define DAMAGE_MODIFICATION_CALLBACK(Type, Multipliers, Context) \
    [&Multipliers, &Context](const TScriptInterface<IIndividualTraitHolder>& TraitHolder) { \
        APPLY_DAMAGE_MODIFICATION(Global, Multipliers, Context, TraitHolder); \
        APPLY_DAMAGE_MODIFICATION(Type, Multipliers, Context, TraitHolder); \
    }

/**
 * Shorthand to handle processing over allies
 * @param Type The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
#define ALLY_DAMAGE_MODIFICATION_CALLBACK(Type, Multipliers, Context) \
    [&Multipliers, &Context](const TScriptInterface<IBattler>& Ally) { \
        Ally->ForEachIndividualTraitHolder(DAMAGE_MODIFICATION_CALLBACK(Type, Multipliers, Context)); \
    }

namespace Traits {

/**
 * Apply the damage modification traits with the given trait information
 * @tparam Member The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 * @param TraitHolder The trait holder object to process 
 */
template <auto Member>
void ApplyDamageModificationTraits(FDamageMultipliers &Multipliers, const FMoveDamageInfo& Context,
    const TScriptInterface<IIndividualTraitHolder>& TraitHolder) {
    auto MatchingTraits = RangeHelpers::CreateRange(TraitHolder->GetDamageModifiers().*Member)
            | std::views::filter([&Context](const UDamageModificationTrait* Trait) { return Trait->MeetsConditions(Context); });
    Algo::ForEach(MatchingTraits, [&Multipliers, &Context](const UDamageModificationTrait* Trait) {
        Trait->Apply(Multipliers, Context);
    });
}

/**
 * Apply the individual damage modifiers for a move with the given context information
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
POKEMONBATTLE_API void ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo& Context);
}