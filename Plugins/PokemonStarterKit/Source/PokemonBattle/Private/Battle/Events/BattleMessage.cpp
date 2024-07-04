// "Unreal Pokémon" created by Retro & Chill.


#include "Battle/Events/BattleMessage.h"

FBattleMessage::FBattleMessage(FText &&Message) : Message(MoveTemp(Message)) {
}

FBattleMessage::FBattleMessage(FText &&Message, const TScriptInterface<IBattleAnimation> &Animation,
    EAnimationPlacement AnimationPlacement) : Message(MoveTemp(Message)), Animation(Animation), AnimationPlacement(AnimationPlacement) {
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