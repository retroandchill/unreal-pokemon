// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Cues/GameplayCue_ApplyMessage.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "Ranges/Optional/Or.h"

bool UGameplayCue_ApplyMessage::HandlesEvent(EGameplayCueEvent::Type EventType) const {
    if (ValidEventTypes.IsEmpty() || ValidEventTypes.Contains(EventType)) {
        return Super::HandlesEvent(EventType);
    }
    
    return false;
}

bool UGameplayCue_ApplyMessage::PerformMessageAppend(AActor *MyTarget, const FGameplayCueParameters &Parameters) const {
    auto CausedAbility = Parameters.EffectContext.GetAbilityInstance_NotReplicated();
    auto Messages = UBattleMessageHelper::FindRunningMessageSet(CausedAbility) |
        UE::Optionals::Or([MyTarget] { return UBattleMessageHelper::FindRunningMessageSet(MyTarget); });
    if (!Messages.IsSet()) {
        return false;
    }

    AppendMessage(MyTarget, Parameters, *Messages);
    return true;
}