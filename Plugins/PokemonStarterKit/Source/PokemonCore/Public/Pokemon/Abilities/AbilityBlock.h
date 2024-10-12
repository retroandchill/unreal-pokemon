// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Lookup/InjectableDependency.h"

#include "AbilityBlock.generated.h"

struct FPokemonDTO;
class IPokemon;

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UAbilityBlock : public UInterface {
    GENERATED_BODY()
};

/**
 * This is the interface used for handling a Pokémon's ability.
 */
class POKEMONCORE_API IAbilityBlock {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
  public:
    /**
     * Initialize a new ability block with the following initializing information
     * @param NewOwner The Pokémon that owns this ability block
     * @param DTO The DTO for the stat block
     */
    virtual void Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) = 0;

    /**
     * Get the named ID of the Pokémon's ability
     * @return The ability ID
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual FName GetAbilityID() const = 0;

    /**
     * Get the display name of the Pokémon's ability
     * @return The display name for the ability
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual const FText &GetDisplayName() const = 0;

    /**
     * Get the description of the given ability
     * @return The description of the ability to display
     */
    UFUNCTION(BlueprintCallable, Category = Abilities)
    virtual const FText &GetAbilityDescription() const = 0;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONCORE_API, IAbilityBlock);