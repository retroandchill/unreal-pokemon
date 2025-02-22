﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Battle.h"
#include "BattleInfo.h"
#include "Subsystems/WorldSubsystem.h"
#include "UE5Coro.h"

#include "BattleTransitionSubsystem.generated.h"

class ABattleTransitionActor;
class ULevelStreamingDynamic;
class ALevelStreamingVolume;
class IBattle;

USTRUCT()
struct POKEMONBATTLE_API FLevelStreamingVolumeState {
    GENERATED_BODY()

    UPROPERTY()
    TObjectPtr<ALevelStreamingVolume> Volume;

    bool bDisabled = false;
};

/**
 * Delegate called when the battle is finished and the player is allowed to continue on
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FBattleFinished, EBattleResult);

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
    void SetBattleMap(const TSoftObjectPtr<UWorld> &NewBattleMap);

    /**
     * Set the battle that is registered to this subystem
     * @param Battle The new battle actor
     */
    void SetRegisteredBattle(const TScriptInterface<IBattle> &Battle);

    /**
     * Initialize a wild battle with the provided Pokémon information
     * @param Info The Pokémon information that should be battled against
     * @param Transition
     * @param
     */
    UE5Coro::TCoroutine<EBattleResult> InitiateBattle(FBattleInfo Info, TSubclassOf<ABattleTransitionActor> Transition,
                                                      FForceLatentCoroutine = {});

  private:
    /**
     * Exit the current battle and return to the field
     */
    UE5Coro::TCoroutine<> ExitBattle(ULevelStreamingDynamic *Battlefield,
                                     const TArray<FLevelStreamingVolumeState> &StreamingStates,
                                     FForceLatentCoroutine = {}) const;

    /**
     * The map to jump to for battle
     */
    UPROPERTY()
    TSoftObjectPtr<UWorld> BattleMap;

    /**
     * The registered battle object for this subsystem
     */
    TWeakInterfacePtr<IBattle> RegisteredBattle;
};