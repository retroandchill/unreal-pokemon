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