// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"

#include "PokemonBattleSettings.generated.h"

/**
 * The information for a stage of stat alteration.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FStatStageInfo {
    GENERATED_BODY()

    /**
     * The amount of the stat will be multiplied by for a positive stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float PositiveStatMultiplier;

    /**
     * The amount of the stat will be multiplied by for a negative stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float NegativeStatMultiplier;

    /**
     * The amount of the accuracy/evasion will be multiplied by for a positive stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, DisplayName = "Positive Accuracy/Evasion Multiplier",
              Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float PositiveAccEvaMultiplier;

    /**
     * The amount of the accuracy/evasion will be multiplied by for a negative stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, DisplayName = "Negative Accuracy/Evasion Multiplier",
              Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float NegativeAccEvaMultiplier;
};

/**
 * Struct that contains information for Gameplay Tags that are applied when HP dips below a certain amount.
 */
USTRUCT(BlueprintType, DisplayName = "HP State Tag")
struct POKEMONBATTLE_API FHPStateTag {
    GENERATED_BODY()

    /**
     * The threshold to apply the tag if HP is under.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|HP",
              meta = (UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f))
    float Threshold;

    /**
     * The tag to apply
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|HP")
    FGameplayTag Tag;
};

/**
 *
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Battle System")
class POKEMONBATTLE_API UPokemonBattleSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The information about how stat stages are altered
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle)
    TArray<FStatStageInfo> StatStages = {
        {3.f / 2.f, 2.f / 3.f, 4.f / 3.f, 3.f / 4.f}, {4.f / 2.f, 2.f / 4.f, 5.f / 3.f, 3.f / 5.f},
        {5.f / 2.f, 2.f / 5.f, 6.f / 3.f, 3.f / 6.f}, {6.f / 2.f, 2.f / 6.f, 7.f / 3.f, 3.f / 7.f},
        {7.f / 2.f, 2.f / 7.f, 8.f / 3.f, 3.f / 8.f}, {8.f / 2.f, 2.f / 8.f, 9.f / 3.f, 3.f / 9.f},
    };

    /**
     * The default ability used for using moves
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle, meta = (MetaClass = "BattleMoveFunctionCode"))
    FSoftClassPath DefaultMoveAbility;

    /**
     * The default ability used for using items in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle, meta = (MetaClass = "BattleItemEffect"))
    FSoftClassPath DefaultBattleItemAbility;

    /**
     * The default level that is streamed in when starting a battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle,
              meta = (AllowedClasses = "/Script/Engine.World"))
    FSoftObjectPath DefaultBattleScene;

    /**
     * The offset of the streamed in battle scene
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle)
    FVector BattleSceneOffset = FVector(0, 0, -5000);

    /**
     * The tags for the HP state of the battler
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, DisplayName = "HP State Tags", Category = Battle)
    TArray<FHPStateTag> HPStateTags;

    /**
     * The damage multiplier for a critical hit
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle)
    float CriticalHitMultiplier = 1.5f;

    /**
     * The critical hit ratios in battle expressed as 1 / the specified value for each number of boosts.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle, meta = (UIMin = 1, ClampMin = 1))
    TArray<int32> CriticalHitRatios = {24, 8, 2, 1};

    /**
     * The multiplier for gaining exp in battle from a trainer Pokémon
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle)
    float TrainerExpGainMultiplier = 1.f;

    /**
     * The priority of a switch action in battle
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = Switching)
    int32 SwitchPriority = 6;

    /**
     * The priority of an item action in battle
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = Switching)
    int32 ItemPriority = 6;

    /**
     * The priority of a switch action in battle
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = Switching,
              meta = (MetaClass = "SwitchActionBase"))
    FSoftClassPath SwitchAbilityClass;
};
