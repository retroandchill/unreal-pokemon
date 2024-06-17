// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/TraitUtilities.h"
#include "Battle/Traits/Damage/DamageModificationTraits.h"
#include "Battle/Traits/CriticalHits/CriticalHitRateModificationTraits.h"

void Traits::ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    auto AllTraits = GetIndividualTraitModifiers<FIndividualDamageModifierTraits>(Context.User, Context.Target);
    for (auto CombinedView = AllTraits | ranges::views::join; const auto &Trait : CombinedView) {
        Trait->Apply(Multipliers, Context);
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