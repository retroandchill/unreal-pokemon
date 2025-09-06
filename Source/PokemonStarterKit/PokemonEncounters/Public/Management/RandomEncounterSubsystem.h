// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"

#include "RandomEncounterSubsystem.generated.h"

struct FGameplayTag;
struct FEncounterEntry;
class AMapEncounterData;

/**
 * Information about an encounter that was fetched.
 */
USTRUCT(BlueprintType)
struct POKEMONENCOUNTERS_API FRetrievedEncounter
{
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
class POKEMONENCOUNTERS_API URandomEncounterSubsystem : public UWorldSubsystem
{
    GENERATED_BODY()

  public:
    /**
     * Set the actor that will convey the encounter data to the world.
     * @param Data The encounter data that is sent to the game
     */
    UFUNCTION(BlueprintCallable, Category = RandomEncounters)
    void SetEncounterData(AMapEncounterData *Data);

    /**
     * Check if there are valid encounters for the given encounter type
     * @param EncounterType The encounter type to search for
     * @return Was an encounter table found?
     */
    UFUNCTION(BlueprintPure, Category = RandomEncounters, meta = (AutoCreateRefTerm = EncounterType))
    bool HasEncountersForType(UPARAM(meta = (Categories = "EncounterTypes")) const FGameplayTag &EncounterType) const;

    UFUNCTION(BlueprintPure, Category = RandomEncounters, meta = (AutoCreateRefTerm = EncounterType))
    bool HasEncountersForTypeExact(UPARAM(meta = (Categories = "EncounterTypes"))
                                       const FGameplayTag &EncounterType) const;

    /**
     * Request a random encounter of the given type.
     * @param EncounterType The encounter type to pull from for the list
     * @param Encounter The encounter that was found
     * @param ChanceRolls The number of rolls to make, it will favor the highest roll made, leading to rarer encounters
     * @return Was an encounter found?
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = RandomEncounters,
              meta = (ExpandBoolAsExecs = "ReturnValue", AutoCreateRefTerm = EncounterType))
    bool RequestEncounterForType(UPARAM(meta = (Categories = "EncounterTypes")) const FGameplayTag &EncounterType,
                                 FRetrievedEncounter &Encounter, int32 ChanceRolls = 1) const;

    UFUNCTION(BlueprintCallable, Category = RandomEncounters, meta = (AutoCreateRefTerm = EncounterType))
    bool CheckEncounterTriggered(ACharacter *PlayerCharacter, const FGameplayTag &EncounterType,
                                 bool bRepelActive = false, bool bTriggeredByStep = true);

    UFUNCTION(BlueprintCallable, Category = RandomEncounters, meta = (AutoCreateRefTerm = EncounterType))
    bool AllowEncounter(const FRetrievedEncounter &Encounter, bool bRepelActive = false);

  private:
    UPROPERTY()
    bool bEncountersDisabled = false;

    UPROPERTY()
    TObjectPtr<AMapEncounterData> EncounterData;

    int32 ChanceAccumulator = 0;
    int32 StepCount = 0;
};
