// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveAnimation.generated.h"

class IBattler;

DECLARE_DELEGATE(FOnMoveAnimationComplete)

UCLASS(Abstract)
class POKEMONBATTLE_API AMoveAnimation : public AActor {
    GENERATED_BODY()

public:
    void Initialize(const TScriptInterface<IBattler>& UserIn, const TArray<TScriptInterface<IBattler>>& TargetsIn);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Scope)
    const TScriptInterface<IBattler>& GetUser() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Scope)
    const TArray<TScriptInterface<IBattler>>& GetTargets() const;

    UFUNCTION(BlueprintImplementableEvent, Category = Display)
    void PlayAnimation();

    UFUNCTION(BlueprintCallable, Category = "Moves|Control Flow")
    void ExecuteOnMoveAnimationComplete() const;

    FOnMoveAnimationComplete& GetOnMoveAnimationComplete();
    
private:
    UPROPERTY(BlueprintGetter = GetUser, Category = Scope)
    TScriptInterface<IBattler> User;

    UPROPERTY(BlueprintGetter = GetTargets, Category = Scope)
    TArray<TScriptInterface<IBattler>> Targets;

    FOnMoveAnimationComplete OnMoveAnimationComplete;
};