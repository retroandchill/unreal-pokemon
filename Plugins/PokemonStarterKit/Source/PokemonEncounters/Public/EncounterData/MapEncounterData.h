// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "MapEncounterData.generated.h"

/**
 * Struct that contains the information about an individual encounter
 */
USTRUCT(BlueprintType)
struct POKEMONENCOUNTERS_API FEncounterEntry {
    GENERATED_BODY()

    /**
     * The chance the Pokémon has of appearing
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RandomEncounters,
              meta = (UIMin = 1, ClampMin = 1, UIMax = 100, ClampMax = 100))
    int32 Chance = 1;

    /**
     * The species that can be encountered
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RandomEncounters,
              meta = (GetOptions = "PokemonData.SpeciesHelper.GetSpeciesNames"))
    FName Species;

    /**
     * The range of levels for that encounter
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RandomEncounters,
              meta = (UIMin = 1, ClampMin = 1))
    FInt32Range LevelRange;
};

/**
 * Struct that contains all the information about a specific encounter type.
 */
USTRUCT(BlueprintType)
struct POKEMONENCOUNTERS_API FEncounterData {
    GENERATED_BODY()

    /**
     * A manual override to the trigger chance for the map.
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RandomEncounters,
              meta = (UIMin = 1, ClampMin = 1, UIMax = 100, ClampMax = 100))
    int32 TriggerChance = 21;

    /**
     * The individual encounters in an area
     */
    UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = RandomEncounters)
    TArray<FEncounterEntry> Encounters;
};

/**
 * Actor that contains all information about a map's random encounters.
 */
UCLASS(BlueprintType, Blueprintable)
class POKEMONENCOUNTERS_API AMapEncounterData : public AActor {
    GENERATED_BODY()

public:
    /**
     * Get the full list of encounters in an area.
     * @return The full list of encounters in an area.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = RandomEncounters)
    const TMap<FGameplayTag, FEncounterData>& GetEncounters() const;
    
#if WITH_EDITOR
    EDataValidationResult IsDataValid(FDataValidationContext& Context) const override;
#endif
    
private:
    /**
     * The full list of encounters in an area.
     */
    UPROPERTY(EditInstanceOnly, BlueprintGetter = GetEncounters, Category = RandomEncounters,
        meta = (GetKeyOptions = "PokemonData.EncounterTypeUtils.GetEncounterTypes"))
    TMap<FGameplayTag, FEncounterData> Encounters;


};