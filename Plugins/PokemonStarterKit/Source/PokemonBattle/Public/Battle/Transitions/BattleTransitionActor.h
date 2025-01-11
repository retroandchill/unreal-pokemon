// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UE5Coro.h"

#include "BattleTransitionActor.generated.h"

/**
 * The actor used to perform the battle transition.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API ABattleTransitionActor : public AActor {
    GENERATED_BODY()

public:
    UE5Coro::TCoroutine<> Execute();

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Transitions")
    void TransitionToBattle();

    UFUNCTION(BlueprintCallable, Category = "Battle|Transitions")
    void CompleteTransition();

  private:
    TSharedRef<TFutureState<int32>> OnBattleTransitionComplete = MakeShared<TFutureState<int32>>();
};