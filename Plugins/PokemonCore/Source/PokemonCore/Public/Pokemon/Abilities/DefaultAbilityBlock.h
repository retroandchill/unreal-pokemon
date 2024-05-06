// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBlock.h"
#include "UObject/Object.h"
#include "DefaultAbilityBlock.generated.h"

class IPokemon;

/**
 * The default implementation of a Pokémon's ability block.
 */
UCLASS()
class POKEMONCORE_API UDefaultAbilityBlock : public UObject, public IAbilityBlock {
    GENERATED_BODY()

public:
    void Initialize(const TScriptInterface<IPokemon> &NewOwner, const FPokemonDTO &DTO) override;
    
    UFUNCTION(BlueprintPure, Category = Abilities)
    FName GetAbilityID() const override;

    UFUNCTION(BlueprintPure, Category = Abilities)
    FText GetDisplayName() const override;

    
    UFUNCTION(BlueprintPure, Category = Abilities)
    FText GetAbilityDescription() const override;
    
private:
    /**
     * Get the numerical index for an ability
     * @return The numerical index for an ability
     */
    int32 GetAbilityIndex() const;
    
    /**
     * A back reference to the Pokémon that owns this ability block
     */
    UPROPERTY(SaveGame)
    TScriptInterface<IPokemon> Owner;

    /**
     * A manual override for the default ability.
     */
    UPROPERTY(SaveGame)
    TOptional<FName> Ability;

};
