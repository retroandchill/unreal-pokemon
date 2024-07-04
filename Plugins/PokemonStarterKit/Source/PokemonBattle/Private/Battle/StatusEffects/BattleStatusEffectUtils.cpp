// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/StatusEffects/BattleStatusEffectUtils.h"
#include "Battle/Battlers/Battler.h"
#include "Battle/Events/BattleMessage.h"

bool UBattleStatusEffectUtils::CanStatusEffectBeInflicted(FName StatusEffectID,
                                                      const UGameplayAbility *SourceAbility,
                                                      const TScriptInterface<IBattler> &Target, FText AlreadyAppliedFormat, FText HasOtherStatusFormat) {
    auto Messages = UBattleMessageHelper::FindRunningMessageSet(SourceAbility);
    if (auto &StatusEffect = Target->GetStatusEffect(); StatusEffect.IsSet()) {
        auto &Status = StatusEffect.GetValue();
        if (Messages != nullptr) {
            if (Status.StatusEffectID == StatusEffectID) {
                auto AppliedAlreadyText = FText::Format(AlreadyAppliedFormat, {
                    {"Pkmn", Target->GetNickname()}
                });
                UBattleMessageHelper::AppendMessage(*Messages, AppliedAlreadyText);
            } else {
                auto HasAnotherStatusText = FText::Format(HasOtherStatusFormat, {
                    {"Pkmn", Target->GetNickname()}
                });
                UBattleMessageHelper::AppendMessage(*Messages, HasAnotherStatusText);
            }
        }
        
        return false;
    }

    return true;
}