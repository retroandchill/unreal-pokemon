// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/BattleStatusEffectUtils.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Battlers/Battler.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

UE5Coro::TCoroutine<bool> UBattleStatusEffectUtils::CanStatusEffectBeInflicted(UE5Coro::TLatentContext<const UObject> Context,
                                                                               FName StatusEffectID,
                                                                               const TScriptInterface<IBattler> &Target, const FText& AlreadyAppliedFormat, const FText& HasOtherStatusFormat) {
    auto &StatusEffect = Target->GetStatusEffect();
    if (!StatusEffect.IsSet()) {
        co_return true;
    }

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get(Context.Target);
    if (auto &Status = StatusEffect.GetValue(); Status.StatusEffectID == StatusEffectID) {
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(AlreadyAppliedFormat, "Pkmn", Target->GetNickname()));
    } else {
        co_await Dispatcher.DisplayMessage(FText::FormatNamed(HasOtherStatusFormat, "Pkmn", Target->GetNickname()));
    }

    co_return false;

}