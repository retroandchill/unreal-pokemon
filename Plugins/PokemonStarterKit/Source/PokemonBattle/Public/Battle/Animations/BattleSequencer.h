// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/BattleMessage.h"
#include "GameFramework/Actor.h"
#include "UE5Coro.h"
#include <queue>

#include "BattleSequencer.generated.h"

UCLASS(Abstract, Blueprintable)
class POKEMONBATTLE_API ABattleSequencer : public AActor {
    GENERATED_BODY()

    static constexpr TCHAR UninitializedLog[] = TEXT("BattleSequencer is not initialized!");

  protected:
    void BeginPlay() override;
    void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  public:
    /**
     * Queues a battle message to be displayed during the battle sequence.
     *
     * @param Text The message to be added to the battle queue.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Sequencer")
    static void QueueBattleMessage(FText Text);

    /**
     * Queues a battle message to be displayed during the battle sequence with an associated animation.
     *
     * @param Text The message to be added to the battle queue.
     * @param Animation The animation to be played with the battle message.
     * @param AnimationPlacement The position where the animation should be played (before or after the message).
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Sequencer")
    static void QueueBattleMessageWithAnimation(FText Text, const TScriptInterface<IBattleAnimation> &Animation,
                                                EAnimationPlacement AnimationPlacement = EAnimationPlacement::Before);

    /**
     * Displays multiple battle messages during the battle sequence.
     */
    static FVoidCoroutine DisplayBattleMessages(const UObject* WorldContext, FForceLatentCoroutine = {});

    /**
     * Adds a battle message to the message queue.
     *
     * @param Args Variadic template arguments representing the battle messages to be added.
     */
    template <typename... A>
        requires std::constructible_from<FBattleMessage, A...>
    void AddBattleMessage(A &&...Args) {
        Messages.emplace(std::forward<A>(Args)...);
    }

    /**
     * Processes battle messages based on the provided variadic arguments.
     */
    UE5Coro::TCoroutine<> ProcessBattleMessages(FForceLatentCoroutine = {});

  protected:
    /**
     * Displays a battle message during the battle sequence.
     *
     * @param Message The battle message to be displayed.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Sequencer")
    void DisplayBattleMessage(const FBattleMessage &Message);

private:
    UE5Coro::TCoroutine<> TryDisplayMessage(FText Message, FForceLatentCoroutine = {}) const;

    static TWeakObjectPtr<ABattleSequencer> Instance;
    std::queue<FBattleMessage> Messages;

    bool bIsProcessingMessages = false;
};