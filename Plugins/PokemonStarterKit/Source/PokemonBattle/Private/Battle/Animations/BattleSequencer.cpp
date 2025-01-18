// "Unreal Pokémon" created by Retro & Chill.

#include "Battle/Animations/BattleSequencer.h"
#include "Battle/Animations/BattleAnimation.h"
#include "PokemonBattleModule.h"
#include "Utilities/PokemonCoroutineDispatcher.h"

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

FVoidCoroutine ABattleSequencer::DisplayBattleMessages(const UObject *WorldContext, FForceLatentCoroutine) {
    if (!Instance.IsValid()) {
        UE_LOG(LogBattle, Warning, UninitializedLog)
        co_return;
    }

    co_await Instance->ProcessBattleMessages();
}

UE5Coro::TCoroutine<> ABattleSequencer::ProcessBattleMessages(FForceLatentCoroutine) {
    if (bIsProcessingMessages) {
        UE_LOG(LogBattle, Warning, TEXT("Battle sequencer is already processing!"))
        co_return;
    }

    bIsProcessingMessages = true;
    while (!Messages.empty()) {
        if (auto &BattleMessage = Messages.front(); BattleMessage.AnimationPlacement == EAnimationPlacement::Before) {
            co_await TryDisplayMessage(BattleMessage.Message);
            co_await IBattleAnimation::PlayAnimation(BattleMessage.Animation);
        } else {
            co_await TryDisplayMessage(BattleMessage.Message);
            co_await IBattleAnimation::PlayAnimation(BattleMessage.Animation);
        }

        Messages.pop();
    }

    bIsProcessingMessages = false;
}

UE5Coro::TCoroutine<> ABattleSequencer::TryDisplayMessage(FText Message, FForceLatentCoroutine) const {
    if (Message.IsEmptyOrWhitespace()) {
        co_return;
    }

    auto &Dispatcher = IPokemonCoroutineDispatcher::Get();
    co_await Dispatcher.DisplayMessage(std::move(Message));
}
