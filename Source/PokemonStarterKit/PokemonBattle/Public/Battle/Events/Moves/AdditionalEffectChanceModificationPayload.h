// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "AdditionalEffectChanceModificationPayload.generated.h"

class IBattler;
class IBattleMove;
/**
 * The struct that holds the actual payload data
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FAdditionalEffectChanceModificationData
{
    GENERATED_BODY()

    /**
     * The user of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = AdditionalEffects)
    TScriptInterface<IBattler> User;

    /**
     * The target of the move
     */
    UPROPERTY(BlueprintReadOnly, Category = AdditionalEffects)
    TScriptInterface<IBattler> Target;

    /**
     * The value of the effect chance. It is floored to an integer before application
     */
    UPROPERTY(BlueprintReadOnly, Category = AdditionalEffects)
    float AdditionalEffectChance;

    /**
     * Default constructor
     */
    FAdditionalEffectChanceModificationData() = default;

    /**
     * Construct a new payload from the given data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AdditionalEffectChance The chance of the effect occurring
     */
    FAdditionalEffectChanceModificationData(const TScriptInterface<IBattler> &User,
                                            const TScriptInterface<IBattler> &Target, int32 AdditionalEffectChance);
};

/**
 * Event payload for modifications to the additional effect chance of a move
 */
UCLASS(BlueprintType)
class POKEMONBATTLE_API UAdditionalEffectChanceModificationPayload : public UObject
{
    GENERATED_BODY()

  public:
    /**
     * Construct a new payload from the given data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AdditionalEffectChance The chance of the effect occurring
     */
    static UAdditionalEffectChanceModificationPayload *Create(const TScriptInterface<IBattler> &User,
                                                              const TScriptInterface<IBattler> &Target,
                                                              int32 AdditionalEffectChance);

    /**
     * Get the housed payload data
     * @return The contained payload data
     */
    UFUNCTION(BlueprintPure, Category = AdditionalEffects)
    const FAdditionalEffectChanceModificationData &GetData() const;

    /**
     * Set the move's additional effect chance to the new value
     * @param AdditionalEffectChance The new value of the additional effect chance
     */
    UFUNCTION(BlueprintCallable, Category = AdditionalEffects)
    void SetAdditionalEffectChance(float AdditionalEffectChance) const;

  private:
    /**
     * The contained payload data
     */
    TSharedPtr<FAdditionalEffectChanceModificationData> Data;
};
