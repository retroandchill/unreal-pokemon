// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/PokemonDTO.h"

#include "PartyManagementHelpers.generated.h"

class IPokemon;
/**
 * Helper class for things related to managing the player's party.
 */
UCLASS()
class POKEMONCORE_API UPartyManagementHelpers : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Add a Pokémon to the player's party.
     * @param WorldContext The world context used to retrieve the game data.
     * @param Pokemon The data used to create the new Pokémon.
     * @return The created Pokémon.
     */
    UFUNCTION(BlueprintCallable, Category = "Player|Pokémon", meta = (WorldContext = "WorldContext"))
    static TScriptInterface<IPokemon> AddPokemonToParty(const UObject* WorldContext, const FPokemonDTO& Pokemon);

};
