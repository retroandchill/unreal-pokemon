// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveExecutor.generated.h"

class IBattleMove;
class IBattler;

DECLARE_DELEGATE_OneParam(FCompleteCheck, bool);
DECLARE_DELEGATE(FOnExecutorDispatchComplete);


/**
 * Actor that is spawned in to perform all the calculations and message display for a move.
 */
UCLASS(Abstract)
class POKEMONBATTLE_API AMoveExecutor : public AActor {
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Moves")
    const TScriptInterface<IBattleMove> &GetMove() const;

    void SetMove(const TScriptInterface<IBattleMove>& MoveIn);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Success Checking")
    void PerformGlobalFailureCheck(const TScriptInterface<IBattler> &User, const TArray<TScriptInterface<IBattler>> &Targets);

    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Success Checking")
    void ValidateTargets(const TScriptInterface<IBattler> &User, const TArray<TScriptInterface<IBattler>> &Targets);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Success Checking")
    void PerformSuccessCheckOnTarget(const TScriptInterface<IBattler>& User, const TScriptInterface<IBattler>& Target, int32 TargetCount = 1);

    UFUNCTION(BlueprintImplementableEvent, Category = "Moves|Success Checking")
    void ProcessMiss();
    
    FCompleteCheck& GetCompleteCheckDelegate();
    FOnExecutorDispatchComplete& GetOnExecutorDispatchComplete();

    UFUNCTION(BlueprintCallable, Category = "Moves|Control Flow")
    void ExecuteCheckComplete(bool bSuccess) const;

    UFUNCTION(BlueprintCallable, Category = "Moves|Control Flow")
    void ExecuteExecutorDispatchComplete() const;
    
private:
    /**
     * The move this is the executor for
     */
    UPROPERTY(BlueprintGetter = GetMove, Category = "Moves")
    TScriptInterface<IBattleMove> Move;
    
    FCompleteCheck CompleteCheck;

    FOnExecutorDispatchComplete OnExecutorDispatchComplete;

};