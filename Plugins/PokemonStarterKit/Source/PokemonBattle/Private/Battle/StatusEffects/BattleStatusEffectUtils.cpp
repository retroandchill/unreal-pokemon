// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/StatusEffects/BattleStatusEffectUtils.h"
#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Battlers/Battler.h"

bool UBattleStatusEffectUtils::CanStatusEffectBeInflicted(FName StatusEffectID,
                                                          const TScriptInterface<IBattler> &Target,
                                                          FText AlreadyAppliedFormat, FText HasOtherStatusFormat) {
    if (auto &StatusEffect = Target->GetStatusEffect(); StatusEffect.IsSet()) {
        if (auto &Status = StatusEffect.GetValue(); Status.StatusEffectID == StatusEffectID) {
            ABattleSequencer::QueueBattleMessage(
                FText::Format(AlreadyAppliedFormat, {{"Pkmn", Target->GetNickname()}}));
        } else {
            ABattleSequencer::QueueBattleMessage(
                FText::Format(HasOtherStatusFormat, {{"Pkmn", Target->GetNickname()}}));
        }

        return false;
    }

    return true;
}