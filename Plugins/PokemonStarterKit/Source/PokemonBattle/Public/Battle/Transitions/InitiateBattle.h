// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battle.h"
#include "BattleInfo.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RetroLib/Async/BlueprintCoroutineActionBase.h"

#include "InitiateBattle.generated.h"

class ABattleTransitionActor;

/**
 * Delegate that is dispatched after battle is complete
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterBattle, EBattleResult, Result);

/**
 * Initiate a battler with the provided settings then after the battle continue script processing
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UInitiateBattle : public UBlueprintCoroutineActionBase {
    GENERATED_BODY()

  public:
    /**
     * Initiate a battler with the provided settings then after the battle continue script processing
     * @param WorldContextObject The object used to obtain the world
     * @param BattleInfo The information about the battle in question
     * @param Transition
     * @return The created async node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Battle, meta = (WorldContext = "WorldContextObject"))
    static UInitiateBattle *InitiateBattle(const UObject *WorldContextObject, const FBattleInfo &BattleInfo,
                                           TSubclassOf<ABattleTransitionActor> Transition);

    protected:
    UE5Coro::TCoroutine<> ExecuteCoroutine(FForceLatentCoroutine) override;;

  private:
    /**
     * This is what is executed after the battle is complete
     */
    UPROPERTY(BlueprintAssignable)
    FAfterBattle AfterBattle;

    /**
     * The information about the battle in question
     */
    UPROPERTY()
    FBattleInfo BattleInfo;

    UPROPERTY()
    TSubclassOf<ABattleTransitionActor> Transition;
};
