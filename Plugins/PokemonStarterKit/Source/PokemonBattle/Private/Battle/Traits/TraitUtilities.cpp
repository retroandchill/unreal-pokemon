// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/TraitUtilities.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTrait.h"
#include "Battle/Traits/Damage/DamageModificationTraits.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTraits.h"
#include "Battle/Traits/Damage/DamageModificationTrait.h"
#include "range/v3/algorithm/for_each.hpp"
#include "range/v3/functional/bind_back.hpp"
#include "range/v3/view/filter.hpp"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"
#include "range/v3/view/concat.hpp"
#include "RangeHelpers.h"
#include "Battle/Traits/IndividualTraitHolder.h"

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
    
    return ranges::views::concat(UserTraits, UserAllyTraits, TargetTraits, TargetAllyTraits)
        | ranges::views::join;
}

void Traits::ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    auto AllTraits = GetIndividualTraitModifiers<FIndividualDamageModifierTraits>(Context.User, Context.Target)
        | ranges::views::filter(ranges::bind_back(&UDamageModificationTrait::MeetsConditions, Context));
    ranges::for_each(AllTraits, ranges::bind_back(&UDamageModificationTrait::Apply, Multipliers, Context));
}

void Traits::ApplyIndividualCriticalHitRateModifications(int32 &Stages,
                                                          ECriticalOverride &Override,
                                                          const TScriptInterface<IBattler> &User,
                                                          const TScriptInterface<IBattler> &Target) {
    auto AllTraits = GetIndividualTraitModifiers<FIndividualCriticalHitRateModifierTraits>(User, Target)
        | ranges::views::filter(ranges::bind_back(&UCriticalHitRateModificationTrait::MeetsConditions, User, Target));
    ranges::for_each(AllTraits, ranges::bind_back(&UCriticalHitRateModificationTrait::Apply, Stages, Override, User, Target));
}