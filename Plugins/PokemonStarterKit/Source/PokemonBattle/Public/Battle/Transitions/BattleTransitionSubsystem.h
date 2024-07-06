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
    /**
     * Set the battle that is registered to this subystem
     * @param Battle The new battle actor
     */
    void SetRegisteredBattle(const TScriptInterface<IBattle>& Battle);

    /**
     * Initialize a wild battle with the provided Pokémon information
     * @param Info The Pokémon information that should be battled against
     * @param BattleMap The information for the battle in question
     */
    UFUNCTION(BlueprintCallable, Category = Battle)
    void InitiateWildBattle(const FBattleInfo& Info, const TSoftObjectPtr<UWorld>& BattleMap);

private:
    UFUNCTION()
    void SetUpBattle();

    void ExitBattle();
    
    /**
     * The registered battle object for this subsystem
     */
    TWeakInterfacePtr<IBattle> RegisteredBattle;

    UPROPERTY()
    TObjectPtr<ULevelStreamingDynamic> Battlefield;

    TOptional<FBattleInfo> BattleInfo;
};
