﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Species/Stat.h"
#include "UE5Coro.h"

#include "StatChangeHelpers.generated.h"

class IBattler;
/**
 * Struct that contains the change to a stat's value
 */
USTRUCT(BlueprintType)
struct FBattleStatChangeValue {
    GENERATED_BODY()

    /**
     * The ID of the stat that is being changed
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Stats",
              meta = (GetOptions = "PokemonData.StatHelper.GetBattleStatNames"))
    FName StatID;

    /**
     * The amount the stat is being changed by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Battle|Stats", meta = (UIMin = 1, ClampMin = 1))
    int32 Change = 1;
};

/**
 * Helper library for working with stat changes and find the correct tag
 */
UCLASS()
class POKEMONBATTLE_API UStatChangeHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    static UE5Coro::TCoroutine<bool> CanRaiseStat(UE5Coro::TLatentContext<const UObject> Context,
                                                  const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat,
                                                  bool bShowMessages = true, bool bIgnoreInversion = false);

    static UE5Coro::TCoroutine<bool> CanLowerStat(UE5Coro::TLatentContext<const UObject> Context,
                                                  const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat,
                                                  bool bShowMessages = true, bool bIgnoreInversion = false);

    /**
     * Get the value of a stat's stage
     * @param Battler The battler to check against
     * @param Stat The stat to check
     * @return The value of the stat's stage
     */
    UFUNCTION(BlueprintPure, Category = "Battle|Stats")
    static int32 GetStatStageValue(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat);

    /**
     * Check if a stat stage is at its maximum value
     * @param Battler The battler to check against
     * @param Stat The stat to check
     * @return Is this stat maxed out
     */
    UFUNCTION(BlueprintPure, Category = "Battle|Stats")
    static bool StatStageAtMax(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat);

    /**
     * Check if a stat stage is at its minimum value
     * @param Battler The battler to check against
     * @param Stat The stat to check
     * @return Is this stat maxed out
     */
    UFUNCTION(BlueprintPure, Category = "Battle|Stats")
    static bool StatStageAtMin(const TScriptInterface<IBattler> &Battler, FMainBattleStatHandle Stat);

    static TOptional<FText> GetStatChangeMessage(const TScriptInterface<IBattler> &Battler, const FText &StatName,
                                                 int32 Change);

    /**
     * Perform a change to a battler's stat stages
     * @param Battler The battler in question to change the stats of
     * @param Stat The stat that was changed
     * @param Stages The number of stages to change the amount by
     * @param Ability
     * @return The actual number of stages that were changed
     */
    static UE5Coro::TCoroutine<int32> ChangeBattlerStatStages(const TScriptInterface<IBattler> &Battler, FName Stat,
                                                              int32 Stages, UGameplayAbility *Ability = nullptr);
};
