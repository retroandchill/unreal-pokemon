// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UtilitiesSubsystem.generated.h"

/**
 * Subsystem that is used to get the utilities objects.
 */
UCLASS()
class POKEMONCORE_API UUtilitiesSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;
    void Deinitialize() override;

    /**
     * Get the Pokémon utilities object
     * @return The object in question
     */
    UObject *GetPokemonUtilities() const;

    /**
     * Get the item utilities object
     * @return The object in question
     */
    UObject *GetItemUtilities() const;

  private:
    /**
     * The Pokémon utilities class object
     */
    UPROPERTY()
    TObjectPtr<UObject> PokemonUtilities;
    
    /**
     * The item utilities class object
     */
    UPROPERTY()
    TObjectPtr<UObject> ItemUtilities;
};
