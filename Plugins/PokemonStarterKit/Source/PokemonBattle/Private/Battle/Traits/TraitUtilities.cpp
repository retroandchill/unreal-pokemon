// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/TraitUtilities.h"
#include "Battle/Traits/Damage/DamageModificationTraits.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTraits.h"
#include "range/v3/view/join.hpp"
#include "range/v3/view/transform.hpp"

void Traits::ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    auto UserTraits = Context.User->GetTraitHolders() | ranges::views::transform(MAKE_DMOD_PAIR(User));
    auto UserAllyTraits =
        Context.User->GetAllies() |
        ranges::views::transform([](const TScriptInterface<IBattler> &Battler) { return Battler->GetTraitHolders(); }) |
        ranges::views::join | ranges::views::transform(MAKE_DMOD_PAIR(UserAllies));

    auto TargetTraits = Context.Target->GetTraitHolders() | ranges::views::transform(MAKE_DMOD_PAIR(Target));
    auto TargetAllyTraits =
        Context.Target->GetAllies() |
        ranges::views::transform([](const TScriptInterface<IBattler> &Battler) { return Battler->GetTraitHolders(); }) |
        ranges::views::join | ranges::views::transform(MAKE_DMOD_PAIR(TargetAllies));

    std::array<ranges::any_view<FTraitDamagePair>, 4> AllTraits = {UserTraits, UserAllyTraits, TargetTraits,
                                                             TargetAllyTraits};
    for (auto CombinedTraits = AllTraits | ranges::views::join; const auto &[Callback, Trait] : CombinedTraits) {
        ApplyDamageModificationTraits<&FIndividualDamageModifierTraits::Global>(Multipliers, Context, Trait);
        Callback(Multipliers, Context, Trait);
    }
}

int32 Traits::ApplyIndividualCriticalHitRateModifications(const TScriptInterface<IBattler> &User,
    const TScriptInterface<IBattler> &Target) {
    auto UserTraits = User->GetTraitHolders() | ranges::views::transform(MAKE_CMOD_PAIR(User));
    auto UserAllyTraits =
        User->GetAllies() |
        ranges::views::transform([](const TScriptInterface<IBattler> &Battler) { return Battler->GetTraitHolders(); }) |
        ranges::views::join | ranges::views::transform(MAKE_CMOD_PAIR(UserAllies));

    auto TargetTraits = Target->GetTraitHolders() | ranges::views::transform(MAKE_CMOD_PAIR(Target));
    auto TargetAllyTraits =
        Target->GetAllies() |
        ranges::views::transform([](const TScriptInterface<IBattler> &Battler) { return Battler->GetTraitHolders(); }) |
        ranges::views::join | ranges::views::transform(MAKE_CMOD_PAIR(TargetAllies));

    std::array<ranges::any_view<FTraitCriticalPair>, 4> AllTraits = {UserTraits, UserAllyTraits, TargetTraits,
                                                             TargetAllyTraits};

    int32 Sum = 0;
    for (auto CombinedTraits = AllTraits | ranges::views::join; const auto &[Callback, Trait] : CombinedTraits) {
        Sum += Callback(User, Target, Trait);
    }

    return Sum;
}