// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "BattleInfo.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleTransitionSubsystem.generated.h"

class ULevelStreamingDynamic;
class IBattle;

/**
 * Subsystem used to control transitions to and from the battle scene
 */
UCLASS()
class POKEMONBATTLE_API UBattleTransitionSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    
    /**
     * Set the battle map to the new map in question
     * @param NewBattleMap The battle map to jump to
     */
    UFUNCTION(BlueprintCallable, Category = Battle)
    void SetBattleMap(const TSoftObjectPtr<UWorld>& NewBattleMap);
    
    /**
     * Set the battle that is registered to this subystem
     * @param Battle The new battle actor
     */
    void SetRegisteredBattle(const TScriptInterface<IBattle>& Battle);

    /**
     * Initialize a wild battle with the provided Pokémon information
     * @param Info The Pokémon information that should be battled against
     */
    UFUNCTION(BlueprintCallable, Category = Battle)
    void InitiateBattle(const FBattleInfo& Info);

private:
    /**
     * Set up the jump into battle
     */
    UFUNCTION()
    void SetUpBattle();

    /**
     * Exit the current battle and return to the field
     */
    void ExitBattle();

    /**
     * The map to jump to for battle
     */
    UPROPERTY()
    TSoftObjectPtr<UWorld> BattleMap;
    
    /**
     * The registered battle object for this subsystem
     */
    TWeakInterfacePtr<IBattle> RegisteredBattle;

    /**
     * The streamed level for the battlefield
     */
    UPROPERTY()
    TObjectPtr<ULevelStreamingDynamic> Battlefield;

    /**
     * The information about the battle in question
     */
    TOptional<FBattleInfo> BattleInfo;
};
