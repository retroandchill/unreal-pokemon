// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Events/BattleMessage.h"
#include "Abilities/GameplayAbility.h"
#include "Battle/Events/RunningMessageSetPayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"

FBattleMessage::FBattleMessage(FText &&Message) : Message(MoveTemp(Message)) {
}

FBattleMessage::FBattleMessage(FText &&Message, const TScriptInterface<IBattleAnimation> &Animation,
    EAnimationPlacement AnimationPlacement) : Message(MoveTemp(Message)), Animation(Animation), AnimationPlacement(AnimationPlacement) {
}

void FBattleMessage::AddReferencedObjects(FReferenceCollector &Collector) {
    Collector.AddReferencedObject(Animation.GetObjectRef());
}

FString FBattleMessage::GetReferencerName() const {
    return TEXT("FBattleMessage");
}

const TArray<FBattleMessage> &UBattleMessageHelper::GetMessages(const FRunningMessageSet &Messages) {
    return *Messages.Messages;
}

void UBattleMessageHelper::AppendMessage(const FRunningMessageSet &Messages, FText Message) {
    Messages.Messages->Emplace(MoveTemp(Message));
}

void UBattleMessageHelper::AppendMessageWithAnimation(const FRunningMessageSet &Messages, FText Message,
    const TScriptInterface<IBattleAnimation> &Animation, EAnimationPlacement AnimationPlacement) {
    Messages.Messages->Emplace(MoveTemp(Message), Animation, AnimationPlacement);
}

const FRunningMessageSet *UBattleMessageHelper::FindRunningMessageSet(const UGameplayAbility *Ability) {
    if (auto EventData = Ability->GetCurrentAbilitySpec()->GameplayEventData; EventData != nullptr) {
        if (auto MessagePayload = Cast<IRunningMessageSetPayload>(EventData->OptionalObject); MessagePayload != nullptr) {
            return &MessagePayload->GetRunningMessageSet();
        }
    }

    if (auto FunctionCode = Cast<UBattleMoveFunctionCode>(Ability); FunctionCode != nullptr) {
        return &FunctionCode->GetRunningMessage();
    }
    
    return nullptr;
}