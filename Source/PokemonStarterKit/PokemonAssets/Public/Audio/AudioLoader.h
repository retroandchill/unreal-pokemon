// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Utilities/PokemonAssetParams.h"

#include "AudioLoader.generated.h"

struct FSpeciesHandle;
class IPokemon;

/**
 * UAudioLoader is a utility class designed for the Pokémon audio system.
 * It facilitates the retrieval of specific audio assets related to Pokémon cries.
 */
UCLASS()
class POKEMONASSETS_API UAudioLoader : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    /**
     * FindPokemonCry retrieves the specific audio cry associated with a
     * given Pokémon object implementing the IPokemon interface.
     *
     * @param Pokemon A TScriptInterface object representing the Pokémon for
     *                which the audio cry is to be retrieved.
     *
     * @return A USoundBase pointer to the sound asset corresponding to the
     *         Pokémon's cry.
     */
    UFUNCTION(BlueprintCallable, Category = "Audio|Pokémon")
    static USoundBase *FindPokemonCry(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Retrieves the specific audio cry associated with a Pokémon species, considering additional parameters.
     *
     * @param Species A handle to the species of the Pokémon for which the audio cry is to be retrieved.
     * @param AdditionalParams Additional parameters, such as gender and shiny status, that influence the specific cry
     * to retrieve.
     *
     * @return A pointer to the USoundBase sound asset corresponding to the Pokémon species' cry, or nullptr if not
     * found.
     */
    UFUNCTION(BlueprintCallable, Category = "Audio|Pokémon",
              meta = (CallableWithoutWorldContext, AutoCreateRefTerm = AdditionalParams))
    static USoundBase *FindSpeciesCry(const FSpeciesHandle &Species,
                                      const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());
};
