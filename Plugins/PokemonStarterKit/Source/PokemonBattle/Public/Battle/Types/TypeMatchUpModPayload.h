// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TypeMatchUpModPayload.generated.h"

class IBattler;

/**
 * Data payload for the type match-up data for the entire type matchup
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FTypeMatchUpModData {
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
    UPROPERTY(BlueprintReadWrite, Category = TypeEffectiveness)
    FName AttackingType;

    /**
     * The multiplier determined by the above types
     */
    UPROPERTY(BlueprintReadWrite, Category = TypeEffectiveness)
    float Multiplier;

    /**
     * Default constructor
     */
    FTypeMatchUpModData() = default;

    /**
     * Create an object from the given data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AttackingType The type of the attacking move
     * @param Multiplier The multiplier determined by the above types
     */
    FTypeMatchUpModData(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, FName AttackingType, float Multiplier);
};

/**
 * Payload object for the event for any additional data needed during damage calculation.
 */
UCLASS()
class POKEMONBATTLE_API UTypeMatchUpModPayload : public UObject {
    GENERATED_BODY()

public:
    /**
     * Create a new payload with the provided event data
     * @param User The user of the move
     * @param Target The target of the move
     * @param AttackingType The type of the attacking move
     * @param Multiplier The multiplier determined by the above types
     * @return The created payload
     */
    static UTypeMatchUpModPayload* Create(const TScriptInterface<IBattler> &User, const TScriptInterface<IBattler> &Target, FName AttackingType, float Multiplier);
    
    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    const FTypeMatchUpModData &GetData() const;

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
    TSharedPtr<FTypeMatchUpModData> Data;

};
