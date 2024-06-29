// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SingleTypeModPayload.generated.h"

/**
 * Data payload for the type match-up data for a single type, type matchup.
 */
USTRUCT(BlueprintType)
struct POKEMONBATTLE_API FSingleTypeModData {
    GENERATED_BODY()

    /**
     * The type of the attacking move
     */
    UPROPERTY(BlueprintReadWrite, Category = TypeEffectiveness)
    FName AttackingType;

    /**
     * The type of the defending Pokémon
     */
    UPROPERTY(BlueprintReadWrite, Category = TypeEffectiveness)
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
     * Constructo a new object with the given payload data
     * @param AttackingType The type of the attacking move
     * @param DefendingType The type of the defending Pokémon
     * @param Multiplier The multiplier determined by the above types
     */
    FSingleTypeModData(FName AttackingType, FName DefendingType, float Multiplier);
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
     * @param AttackingType The type of the attacking move
     * @param DefendingType The type of the defending Pokémon
     * @param Multiplier The multiplier determined by the above types
     * @return The created payload
     */
    static USingleTypeModPayload* Create(FName AttackingType, FName DefendingType, float Multiplier);
    
    /**
     * Get the wrapped payload struct
     * @return The wrapped struct for the payload
     */
    UFUNCTION(BlueprintPure, Category = GameplayEvents)
    FSingleTypeModData& GetData() const;

private:
    /**
     * The wrapped struct for the payload
     */
    TSharedPtr<FSingleTypeModData> Data;

};
