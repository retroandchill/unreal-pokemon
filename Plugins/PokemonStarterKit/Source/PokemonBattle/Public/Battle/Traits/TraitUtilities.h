// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Algo/ForEach.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Moves/BattleDamage.h"
#include "Damage/DamageModificationTrait.h"
#include "IndividualTraitHolder.h"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/concat.hpp"
#include "RangeHelpers.h"
#include "TraitScopes.h"
#include "CriticalHits/CriticalHitRateModificationTrait.h"
#include "Damage/DamageModificationTraits.h"

#define DMOD_TRAITS(Type) &FIndividualDamageModifierTraits::Type

#define APPLY_DMOD(Type) &Traits::ApplyDamageModificationTraits<DMOD_TRAITS(Type)>

#define MAKE_DMOD_PAIR(Type)                                                                                           \
    [](const IIndividualTraitHolder *TraitHolder) {                                                                    \
        return std::make_pair(Traits::FTraitDamageCallback(APPLY_DMOD(Type)), TraitHolder);                                  \
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

#define CMOD_TRAITS(Type) &FIndividualCriticalHitRateModifierTraits::Type

#define APPLY_CMOD(Type) &Traits::ApplyCriticalHitRateModificationTraits<CMOD_TRAITS(Type)>

#define MAKE_CMOD_PAIR(Type)                                                                                           \
    [](const IIndividualTraitHolder *TraitHolder) {                                                                    \
        return std::make_pair(Traits::FTraitCriticalCallback(APPLY_CMOD(Type)), TraitHolder);                                  \
    }

namespace Traits {

using FTraitDamageCallback =
    TFunctionRef<void(FDamageMultipliers &, const FMoveDamageInfo &, const IIndividualTraitHolder *TraitHolder)>;

using FTraitDamagePair = std::pair<FTraitDamageCallback, const IIndividualTraitHolder *>;

using FTraitCriticalCallback =
    TFunctionRef<int32(const TScriptInterface<IBattler>&, const TScriptInterface<IBattler>&, const IIndividualTraitHolder *TraitHolder)>;

using FTraitCriticalPair = std::pair<FTraitCriticalCallback, const IIndividualTraitHolder *>;

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


template <auto Member>
int32 ApplyCriticalHitRateModificationTraits(const TScriptInterface<IBattler>& User,
                                   const TScriptInterface<IBattler>& Target, const IIndividualTraitHolder *TraitHolder) {
    auto MatchingTraits = RangeHelpers::CreateRange(TraitHolder->GetCriticalHitRateModifiers().*Member) |
                          ranges::views::filter([&User, &Target](const UCriticalHitRateModificationTrait *Trait) {
                              return Trait->MeetsConditions(User, Target);
                          });

    return Algo::Accumulate(MatchingTraits, 0, [&User, Target](const UCriticalHitRateModificationTrait *Trait) {
        return Trait->Apply(User, Target);
    });
}

template <typename T, EIndividualTraitScope Scope>
FORCEINLINE auto GetTraitsForScope(const T& Traits) {
    return ranges::views::concat(
        RangeHelpers::CreateRange(Traits.template GetTraitsForScope<EIndividualTraitScope::Global>()),
        RangeHelpers::CreateRange(Traits.template GetTraitsForScope<Scope>())
    );
}

template <typename T>
auto GetIndividualTraitModifiers(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target) {
    auto BattlerTraitHolders = [](const TScriptInterface<IBattler> &Battler) { return Battler->GetTraitHolders(); };
    auto UserTraits = User->GetTraitHolders()
    | ranges::views::transform(&IIndividualTraitHolder::GetIndividualTraits<T>)
    | ranges::views::transform(&GetTraitsForScope<T, EIndividualTraitScope::User>);
    auto UserAllyTraits =
        User->GetAllies() |
        ranges::views::transform(BattlerTraitHolders)
        | ranges::views::join
        | ranges::views::transform(&IIndividualTraitHolder::GetIndividualTraits<T>)
        | ranges::views::transform(&GetTraitsForScope<T, EIndividualTraitScope::UserAlly>);

    auto TargetTraits = Target->GetTraitHolders()
    | ranges::views::transform(&IIndividualTraitHolder::GetIndividualTraits<T>)
    | ranges::views::transform(&GetTraitsForScope<T, EIndividualTraitScope::TargetAlly>);
    auto TargetAllyTraits =
        Target->GetAllies() |
            ranges::views::transform(BattlerTraitHolders) 
            | ranges::views::join | ranges::views::transform(&IIndividualTraitHolder::GetIndividualTraits<T>)
            | ranges::views::transform(&GetTraitsForScope<T, EIndividualTraitScope::TargetAlly>);
    
    return ranges::views::concat(UserTraits, UserAllyTraits, TargetTraits, TargetAllyTraits);
}

POKEMONBATTLE_API int32 ApplyIndividualCriticalHitRateModifications(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target);

} // namespace Traits