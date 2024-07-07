// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleInfo.h"
#include "Battle/Battle.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "InitiateBattle.generated.h"

/**
 * Delegate that is dispatched after battle is complete
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAfterBattle, EBattleResult, Result);

/**
 * Initiate a battler with the provided settings then after the battle continue script processing
 */
UCLASS(meta = (HideThen))
class POKEMONBATTLE_API UInitiateBattle : public UBlueprintAsyncActionBase {
    GENERATED_BODY()

public:
    /**
     * Initiate a battler with the provided settings then after the battle continue script processing
     * @param WorldContext The object used to obtain the world
     * @param BattleInfo The information about the battle in question
     * @return The created async node
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = Battle, meta = (WorldContext = "WorldContext"))
    static UInitiateBattle* InitiateBattle(const UObject* WorldContext, const FBattleInfo& BattleInfo);

    void Activate() override;

private:
    /**
     * Call the action to the battle complete result and unbind the bound delegate
     * @param Result The result of the battle in question
     */
    void OnBattleComplete(EBattleResult Result);
    
    /**
     * This is what is executed after the battle is complete
     */
    UPROPERTY(BlueprintAssignable)
    FAfterBattle AfterBattle;
    
    /**
     * The object used to obtain the world
     */
    UPROPERTY()
    TObjectPtr<const UObject> WorldContext;
    
    /**
     * The information about the battle in question
     */
    UPROPERTY()
    FBattleInfo BattleInfo;

    /**
     * The handle tied to the output exec pin
     */
    TOptional<FDelegateHandle> OutputExecHandle;
    
};
