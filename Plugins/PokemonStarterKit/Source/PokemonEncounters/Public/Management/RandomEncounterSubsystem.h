// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RandomEncounterSubsystem.generated.h"

struct FEncounterEntry;
class AMapEncounterData;

/**
 * Information about an encounter that was fetched.
 */
USTRUCT(BlueprintType)
struct POKEMONENCOUNTERS_API FRetrievedEncounter {
    GENERATED_BODY()

    /**
     * The species for the encounter
     */
    UPROPERTY(BlueprintReadOnly, Category = RandomEncounters)
    FName Species;

    /**
     * The level for the encounter
     */
    UPROPERTY(BlueprintReadOnly, Category = RandomEncounters)
    int32 Level;
};

/**
 * Subsystem that handles the random encounters in the world.
 */
UCLASS()
class POKEMONENCOUNTERS_API URandomEncounterSubsystem : public UWorldSubsystem {
    GENERATED_BODY()

public:
    /**
     * Set the actor that will convey the encounter data to the world.
     * @param Data The encounter data that is sent to the game
     */
    UFUNCTION(BlueprintCallable, Category = RandomEncounters)
    void SetEncounterData(AMapEncounterData* Data);

    /**
     * Request a random encounter of the given type.
     * @param EncounterType The encounter type to pull from for the list
     * @param Encounter The encounter that was found
     * @param ChanceRolls The number of rolls to make, it will favor the highest roll made, leading to rarer encounters
     * @return Was an encounter found?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = RandomEncounters,
        meta = (ExpandBoolAsExecs = "ReturnValue"))
    bool RequestEncounterForType(FName EncounterType, FRetrievedEncounter& Encounter, int32 ChanceRolls = 1) const;

private:
    UPROPERTY()
    TObjectPtr<AMapEncounterData> EncounterData;

};
