// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Algo/ForEach.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Damage/DamageModificationTrait.h"
#include "IndividualTraitHolder.h"
#include "range/v3/view/filter.hpp"
#include "RangeHelpers.h"

#define DMOD_TRAITS(Type) &FIndividualDamageModifierTraits::Type

#define APPLY_DMOD(Type) &Traits::ApplyDamageModificationTraits<DMOD_TRAITS(Type)>

#define MAKE_DMOD_PAIR(Type)                                                                                           \
    [](const IIndividualTraitHolder *TraitHolder) {                                                                    \
        return std::make_pair(Traits::FTraitCallback(APPLY_DMOD(Type)), TraitHolder);                                  \
    }

/**
 * Apply the damage modification traits with the given trait information
 * @param Type The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 * @param TraitHolder The trait holder object to process
 */
#define APPLY_DAMAGE_MODIFICATION(Type, Multipliers, Context, TraitHolder)                                             \
    Traits::ApplyDamageModificationTraits<&FIndividualDamageModifierTraits::Type>(Multipliers, Context, TraitHolder)

/**
 * Shorthand to create a macro for iterating over trait holders
 * @param Type The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
#define DAMAGE_MODIFICATION_CALLBACK(Type, Multipliers, Context)                                                       \
    [&Multipliers, &Context](IIndividualTraitHolder *TraitHolder) {                                                    \
        APPLY_DAMAGE_MODIFICATION(Global, Multipliers, Context, TraitHolder);                                          \
        APPLY_DAMAGE_MODIFICATION(Type, Multipliers, Context, TraitHolder);                                            \
    }

namespace Traits {

using FTraitCallback =
    TFunctionRef<void(FDamageMultipliers &, const FMoveDamageInfo &, const IIndividualTraitHolder *TraitHolder)>;

using FTraitPair = std::pair<FTraitCallback, const IIndividualTraitHolder *>;

/**
 * Apply the damage modification traits with the given trait information
 * @tparam Member The member of the damage modification traits struct to pull from
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 * @param TraitHolder The trait holder object to process
 */
template <auto Member>
void ApplyDamageModificationTraits(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context,
                                   const IIndividualTraitHolder *TraitHolder) {
    auto MatchingTraits = RangeHelpers::CreateRange(TraitHolder->GetDamageModifiers().*Member) |
                          ranges::views::filter([&Context](const UDamageModificationTrait *Trait) {
                              return Trait->MeetsConditions(Context);
                          });
    Algo::ForEach(MatchingTraits, [&Multipliers, &Context](const UDamageModificationTrait *Trait) {
        Trait->Apply(Multipliers, Context);
    });
}

/**
 * Apply the individual damage modifiers for a move with the given context information
 * @param Multipliers The multipliers that have already been applied and will be modified further
 * @param Context The context of the move usage
 */
POKEMONBATTLE_API void ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers,
                                                          const FMoveDamageInfo &Context);
} // namespace Traits