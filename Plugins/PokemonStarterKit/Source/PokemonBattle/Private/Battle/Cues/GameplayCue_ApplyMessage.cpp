// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Cues/GameplayCue_ApplyMessage.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Events/RunningMessageSetPayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"


bool UGameplayCue_ApplyMessage::PerformMessageAppend(AActor *MyTarget, const FGameplayCueParameters &Parameters) const {
    auto CausedAbility = Parameters.EffectContext.GetAbilityInstance_NotReplicated();
    if (auto EventData = CausedAbility->GetCurrentAbilitySpec()->GameplayEventData; EventData != nullptr) {
        if (auto MessagePayload = Cast<IRunningMessageSetPayload>(EventData->OptionalObject); MessagePayload != nullptr) {
            AppendMessage(MyTarget, MessagePayload->GetRunningMessageSet());
            return true;
        }
    }

    if (auto FunctionCode = Cast<UBattleMoveFunctionCode>(CausedAbility); FunctionCode != nullptr) {
        AppendMessage(MyTarget, FunctionCode->GetRunningMessage());
        return true;
    }
    
    return false;
}