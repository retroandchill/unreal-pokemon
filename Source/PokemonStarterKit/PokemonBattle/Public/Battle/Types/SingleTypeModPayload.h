﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "SingleTypeModPayload.generated.h"

class IBattler;
/**
 * Data payload for the type match-up data for a single type, type match up.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FSingleTypeModData {
    GENERATED_BODY()

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
     * The type of the attacking move
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    FName AttackingType;

    /**
     * The type of the defending Pokémon
     */
    UPROPERTY(BlueprintReadOnly, Category = TypeEffectiveness)
    FName DefendingType;

    /**
     * The multiplier determined by the above types
     */
    UPROPERTY(BlueprintReadWrite, Category = TypeEffectiveness)
    float Multiplier;

    /**
     * Default constructor
     */
    FSingleTypeModData() = default;

    /**
     * Constructor a new object with the given payload data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AttackingType The type of the attacking move
     * @param DefendingType The type of the defending Pokémon
     * @param Multiplier The multiplier determined by the above types
     */
    FSingleTypeModData(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target,
                       FName AttackingType, FName DefendingType, float Multiplier);
};

/**
 * Payload for an object that contains a struct containing the attacking and defending types, as well as the calculated
 * match-up.
 */
UCLASS()
class POKEMONBATTLE_API USingleTypeModPayload : public UObject {
    GENERATED_BODY()

  public:
    /**
     * Create a new payload with the provided event data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AttackingType The type of the attacking move
     * @param DefendingType The type of the defending Pokémon
     * @param Multiplier The multiplier determined by the above types
     * @return The created payload
     */
    static USingleTypeModPayload *Create(const TScriptInterface<IBattler> &User,
                                         const TScriptInterface<IBattler> &Target, FName AttackingType,
                                         FName DefendingType, float Multiplier);

    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    const FSingleTypeModData &GetData() const;

    /**
     * Set the multiplier to the given value
     * @param Multiplier The new value of the multiplier
     */
    UFUNCTION(BlueprintCallable, Category = GameplayEvents)
    void SetMultiplier(float Multiplier) const;

  private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FSingleTypeModData> Data;
};
