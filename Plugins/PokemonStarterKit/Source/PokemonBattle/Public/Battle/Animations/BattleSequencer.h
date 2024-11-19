// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "PokemonBattleModule.h"
#include "Battle/Events/BattleMessage.h"
#include "GameFramework/Actor.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/Functional/Delegates.h"
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
    static void QueueBattleMessageWithAnimation(FText Text,
        const TScriptInterface<IBattleAnimation> &Animation,
        EAnimationPlacement AnimationPlacement = EAnimationPlacement::Before);

    /**
     * Displays multiple battle messages during the battle sequence.
     *
     * @param Args Variadic template arguments representing the battle messages to be displayed.
     */
    template <typename... A>
        requires UE::Ranges::CanAddToDelegate<FSimpleMulticastDelegate, A...>
    static void DisplayBattleMessages(A&&... Args) {
        if (Instance.IsValid()) {
            Instance->ProcessBattleMessages(std::forward<A>(Args)...);
        } else {
            // If we don't have an initialized battle sequencer, then we need to short circuit this and create the
            // delegate and immediately execute it to avoid a soft-lock. We can't use std::invoke here because the
            // argument order isn't always correct to do that.
            UE_LOG(LogBattle, Warning, UninitializedLog)
            UE::Ranges::CreateDelegate<FSimpleDelegate>(std::forward<A>(Args)...).Execute();
        }
    }
    
    /**
     * Adds a battle message to the message queue.
     *
     * @param Args Variadic template arguments representing the battle messages to be added.
     */
    template <typename... A>
        requires std::constructible_from<FBattleMessage, A...>
    void AddBattleMessage(A&&... Args) {
        Messages.emplace(std::forward<A>(Args)...);
    }

    /**
     * Processes battle messages based on the provided variadic arguments.
     *
     * @param Args Variadic template arguments representing the battle messages to be processed.
     */
    template <typename... A>
        requires UE::Ranges::CanAddToDelegate<FSimpleMulticastDelegate, A...>
    void ProcessBattleMessages(A&&... Args) {
        UE::Ranges::AddToDelegate(OnMessagesComplete, std::forward<A>(Args)...);
        if (Messages.empty()) {
            ProcessMessagesComplete();
            return;
        }
        
        bIsProcessingMessages = true;
        DisplayBattleMessage(Messages.front());
    }

protected:
    /**
     * Displays a battle message during the battle sequence.
     *
     * @param Message The battle message to be displayed.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Sequencer")
    void DisplayBattleMessage(const FBattleMessage &Message);

    /**
     * Processes the next battle message in the queue.
     *
     * If there are no more messages in the queue, it triggers the completion process.
     * Otherwise, it displays the next battle message.
     */
    UFUNCTION(BlueprintCallable, Category = "Battle|Sequencer")
    void ProcessNextBattleMessage();

private:
    void ProcessMessagesComplete();
    
    static TWeakObjectPtr<ABattleSequencer> Instance;
    std::queue<FBattleMessage> Messages;

    FSimpleMulticastDelegate OnMessagesComplete;
    bool bIsProcessingMessages = false;

};