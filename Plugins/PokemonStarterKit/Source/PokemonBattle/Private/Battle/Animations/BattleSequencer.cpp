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
    if (ensure(Instance.IsValid())) {
        Instance->AddBattleMessage(std::move(Text));
    }
}

void ABattleSequencer::QueueBattleMessageWithAnimation(FText Text, const TScriptInterface<IBattleAnimation> &Animation,
    EAnimationPlacement AnimationPlacement) {
    if (ensure(Instance.IsValid())) {
        Instance->AddBattleMessage(std::move(Text), Animation, AnimationPlacement);
    }
}

void ABattleSequencer::ProcessNextBattleMessage() {
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
