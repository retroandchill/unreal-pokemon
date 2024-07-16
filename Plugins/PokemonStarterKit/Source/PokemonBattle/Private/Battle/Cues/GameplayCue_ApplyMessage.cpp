// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Cues/GameplayCue_ApplyMessage.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Events/RunningMessageSetPayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"

bool UGameplayCue_ApplyMessage::PerformMessageAppend(AActor *MyTarget, const FGameplayCueParameters &Parameters) const {
    auto CausedAbility = Parameters.EffectContext.GetAbilityInstance_NotReplicated();
    auto Messages = UBattleMessageHelper::FindRunningMessageSet(CausedAbility);
    if (Messages == nullptr) {
        return false;
    }

    AppendMessage(MyTarget, Parameters, *Messages);
    return true;
}