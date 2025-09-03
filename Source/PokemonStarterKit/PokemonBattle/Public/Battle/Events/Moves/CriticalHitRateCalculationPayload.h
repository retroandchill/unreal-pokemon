// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/TargetedMoveEventPayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "UObject/Object.h"

#include "CriticalHitRateCalculationPayload.generated.h"

enum class ECriticalOverride : uint8;
class IBattler;
class IBattleMove;

/**
 * The struct that holds the actual payload data
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FCriticalHitRateCalculationData {
    GENERATED_BODY()

    /**
     * The invoking move
     */
    UPROPERTY(BlueprintReadOnly, Category = GameplayEvents)
    TScriptInterface<IBattleMove> Move;

    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    TScriptInterface<IBattler> User;

    /**
     * The target of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    TScriptInterface<IBattler> Target;

    /**
     * Should this effect override any critical hit information?
     */
    UPROPERTY(BlueprintReadWrite, Category = GameplayEvents)
    ECriticalOverride Override = ECriticalOverride::Normal;

    /**
     * How many critical hit stages should this effect raise (or lower) the rate by?
     */
    UPROPERTY(BlueprintReadWrite, Category = GameplayEvents)
    int32 CriticalHitRateStages = 0;

    /**
     * Default constructor
     */
    FCriticalHitRateCalculationData() = default;

    /**
     * Construct a new payload from the given data
     * @param Move The invoking move
     * @param User The user of the move
     * @param Target The target of the move
     * @param Override Any messages to be down after processing.
     * @param CriticalHitRateStages Should messages be shown?
     */
    FCriticalHitRateCalculationData(const TScriptInterface<IBattleMove> &Move, const TScriptInterface<IBattler> &User,
                                    const TScriptInterface<IBattler> &Target, ECriticalOverride Override,
                                    int32 CriticalHitRateStages = 0);
};

/**
 * Event payload object for handling the critical hit calculations.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UCriticalHitRateCalculationPayload : public UObject, public ITargetedMoveEventPayload {
    GENERATED_BODY()

  public:
    /**
     * Create a new payload with the provided event data
     * @param Move The invoking move
     * @param User The user of the move
     * @param Target The target of the move
     * @param Override Any messages to be down after processing.
     * @param CriticalHitRateStages Should messages be shown?
     * @return The created payload
     */
    static UCriticalHitRateCalculationPayload *Create(const TScriptInterface<IBattleMove> &Move,
                                                      const TScriptInterface<IBattler> &User,
                                                      const TScriptInterface<IBattler> &Target,
                                                      ECriticalOverride Override, int32 CriticalHitRateStages = 0);

    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    const FCriticalHitRateCalculationData &GetData() const;

    const TScriptInterface<IBattler> &GetUser() const final;
    const TScriptInterface<IBattler> &GetTarget() const final;

    /**
     * Set the new override state of the move. Applies the in-built priority system for the override.
     * @param Override The new override state
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetCriticalHitRateOverride(ECriticalOverride Override) const;

    /**
     * Set the number of stages for the move
     * @param Stages The number of stages for the move
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetCriticalHitRateStages(int32 Stages) const;

  private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FCriticalHitRateCalculationData> Data;
};
