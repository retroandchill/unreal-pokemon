// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleTransitionActor.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnBattleTransitionComplete)

/**
 * The actor used to perform the battle transition.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API ABattleTransitionActor : public AActor {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintImplementableEvent, Category = "Battle|Transitions")
    void TransitionToBattle();

    FDelegateHandle BindToOnComplete(FOnBattleTransitionComplete::FDelegate&& Binding);

protected:
    UFUNCTION(BlueprintCallable, Category = "Battle|Transitions")
    void CompleteTransition();

private:
    FOnBattleTransitionComplete OnBattleTransitionComplete;


};