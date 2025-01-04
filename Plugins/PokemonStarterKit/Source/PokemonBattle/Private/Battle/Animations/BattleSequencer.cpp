// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/BattleSequencer.h"
#include "PokemonBattleModule.h"

TWeakObjectPtr<ABattleSequencer> ABattleSequencer::Instance;

void ABattleSequencer::BeginPlay() {
    Super::BeginPlay();
    if (!Instance.IsValid()) {
        Instance = this;
    } else {
        UE_LOG(LogBattle, Warning, TEXT("BattleSequencer already exists in the world!"));
    }
}

void ABattleSequencer::EndPlay(const EEndPlayReason::Type EndPlayReason) {
    Super::EndPlay(EndPlayReason);
    Instance = nullptr;
}

void ABattleSequencer::QueueBattleMessage(FText Text) {
    if (Instance.IsValid()) {
        Instance->AddBattleMessage(std::move(Text));
    } else {
        UE_LOG(LogBattle, Warning, UninitializedLog)
    }
}

void ABattleSequencer::QueueBattleMessageWithAnimation(FText Text, const TScriptInterface<IBattleAnimation> &Animation,
                                                       EAnimationPlacement AnimationPlacement) {
    if (Instance.IsValid()) {
        Instance->AddBattleMessage(std::move(Text), Animation, AnimationPlacement);
    } else {
        UE_LOG(LogBattle, Warning, UninitializedLog)
    }
}

void ABattleSequencer::ProcessNextBattleMessage() {
    check(!Messages.empty())
    Messages.pop();
    
    if (Messages.empty()) {
        ProcessMessagesComplete();
    } else {
        DisplayBattleMessage(Messages.front());
    }
}

void ABattleSequencer::ProcessMessagesComplete() {
    auto MessagesDelegate = std::move(OnMessagesComplete);
    MessagesDelegate.Broadcast();
}
