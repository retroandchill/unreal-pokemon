// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Traits/TraitUtilities.h"

void Traits::ApplyIndividualDamageModifications(FDamageMultipliers &Multipliers, const FMoveDamageInfo &Context) {
    auto UserTraits = Context.User->GetTraitHolders()
        | std::ranges::views::transform(MAKE_DMOD_PAIR(User));
    auto UserAllyTraits = Context.User->GetAllies()
        | std::ranges::views::transform([](const TScriptInterface<IBattler>& Battler) { return Battler->GetTraitHolders(); })
        | std::ranges::views::join
        | std::ranges::views::transform(MAKE_DMOD_PAIR(UserAllies));

    auto TargetTraits = Context.Target->GetTraitHolders()
        | std::ranges::views::transform(MAKE_DMOD_PAIR(Target));
    auto TargetAllyTraits = Context.Target->GetAllies()
        | std::ranges::views::transform([](const TScriptInterface<IBattler>& Battler) { return Battler->GetTraitHolders(); })
        | std::ranges::views::join
        | std::ranges::views::transform(MAKE_DMOD_PAIR(TargetAllies));
    
    std::array<ranges::any_view<FTraitPair>, 4> AllTraits = { UserTraits, UserAllyTraits, TargetTraits, TargetAllyTraits };
    for (auto CombinedTraits = AllTraits | std::ranges::views::join; const auto &[Callback, Trait] : CombinedTraits) {
        ApplyDamageModificationTraits<&FIndividualDamageModifierTraits::Global>(Multipliers, Context, Trait);
        Callback(Multipliers, Context, Trait);
    }
}