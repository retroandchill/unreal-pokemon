// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Battle/Events/TargetedMoveEventPayload.h"
#include "Battle/Moves/BattleMoveFunctionCode.h"
#include "UObject/Object.h"

#include "DamageModificationPayload.generated.h"

/**
 * Information about a move during damage modification
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FDamageCalculationData {
    GENERATED_BODY()

    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    TScriptInterface<IBattler> User;

    /**
     * The target of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    TScriptInterface<IBattler> Target;

    /**
     * The total number of targets being hit
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    int32 TargetCount;

    /**
     * The calculated type of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    FName Type;

    /**
     * The calculated base damage value of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = Damage)
    int32 BasePower;

    /**
     * The factor to multiply the move's power by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float PowerMultiplier = 1.0f;

    /**
     * The factor to multiply the user's attack by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float AttackMultiplier = 1.0f;

    /**
     * The factor to multiply the target's defense by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float DefenseMultiplier = 1.0f;

    /**
     * The factor to multiply the final damage output by
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
    float FinalDamageMultiplier = 1.0f;

    /**
     * Default constructor for the struct
     */
    FDamageCalculationData() = default;

    /**
     * Create a basic structure with the specified values
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The total number of available targets
     * @param Type The determined type of the move
     * @param BasePower The base power of the move
     */
    FDamageCalculationData(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                           int32 TargetCount, FName Type, int32 BasePower);
};

/**
 * The payload used during damage modification.
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UDamageModificationPayload : public UObject, public ITargetedMoveEventPayload {
    GENERATED_BODY()

  public:
    /**
     * Create a new payload with the provided event data
     * @param User The user of the move
     * @param Target The target of the move
     * @param TargetCount The total number of available targets
     * @param Type The determined type of the move
     * @param BasePower The base power of the move
     */
    static UDamageModificationPayload *Create(const TScriptInterface<IBattler> &User,
                                              const TScriptInterface<IBattler> &Target, int32 TargetCount, FName Type,
                                              int32 BasePower);

    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    const FDamageCalculationData &GetData() const;

    const TScriptInterface<IBattler> &GetUser() const final;
    const TScriptInterface<IBattler> &GetTarget() const final;

    /**
     * Set the power multiplier
     * @param Amount The factor to multiply the move's power by
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetPowerMultiplier(float Amount) const;

    /**
     * Set the attack multiplier
     * @param Amount The factor to multiply the user's attack by
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetAttackMultiplier(float Amount) const;

    /**
     * Set the defense multiplier
     * @param Amount The factor to multiply the target's defense by
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SeDefenseMultiplier(float Amount) const;

    /**
     * Set the final damage multiplier
     * @param Amount TThe factor to multiply the final damage output by
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetFinalDamageMultiplier(float Amount) const;

  private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FDamageCalculationData> Data;
};
