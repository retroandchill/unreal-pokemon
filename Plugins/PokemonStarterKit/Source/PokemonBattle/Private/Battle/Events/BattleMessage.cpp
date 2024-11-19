// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Events/BattleMessage.h"
#include "Battle/Effects/TurnBasedGameplayEffectComponent.h"

FBattleMessage::FBattleMessage(FText &&Message) : Message(std::move(Message)) {
}

FBattleMessage::FBattleMessage(FText &&Message, const TScriptInterface<IBattleAnimation> &Animation,
                               EAnimationPlacement AnimationPlacement)
    : Message(std::move(Message)), Animation(Animation), AnimationPlacement(AnimationPlacement) {
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
    Messages.Messages->Emplace(std::move(Message));
}

void UBattleMessageHelper::AppendMessageWithAnimation(const FRunningMessageSet &Messages, FText Message,
                                                      const TScriptInterface<IBattleAnimation> &Animation,
                                                      EAnimationPlacement AnimationPlacement) {
    Messages.Messages->Emplace(std::move(Message), Animation, AnimationPlacement);
}

TOptional<const FRunningMessageSet &> UBattleMessageHelper::FindRunningMessageSet(const UGameplayAbility *Ability) {
    return nullptr;
}

TOptional<const FRunningMessageSet &> UBattleMessageHelper::FindRunningMessageSet(const AActor *Actor) {
    return nullptr;
}