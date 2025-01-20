// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Graphics/BattleRender.h"
#include "Images/ImageAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Utilities/PokemonAssetParams.h"

#include "SpriteLoader.generated.h"

class ITrainer;
class IPokemon;
class UPaperFlipbook;

/**
 * Blueprint Function library for loading sprites dynamically.
 */
UCLASS()
class POKEMONASSETS_API USpriteLoader : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    /**
     * Get the battle sprite for a given Pokémon.
     * @param Pokemon The Pokémon to get the battle sprite for.
     * @param bBack Whether to retrieve the back sprite (true) or the front sprite (false).
     * @return The graphical asset for the specified Pokémon's battle sprite.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static FBattleRender GetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack = false);

    UFUNCTION(BlueprintCallable, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext, Latent, LatentInfo = LatentActionInfo))
    static FVoidCoroutine AsyncGetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, FBattleRender& OutRender, bool bBack = false, FLatentActionInfo LatentActionInfo = FLatentActionInfo());
    
    static UE5Coro::TCoroutine<FBattleRender> AsyncGetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack = false, FForceLatentCoroutine = {});

    /**
     * Retrieve the battle sprite for a given Pokémon species.
     * @param Species The species of Pokémon to get the battle sprite for.
     * @param bBack Whether to retrieve the back sprite (true) or the front sprite (false).
     * @param AdditionalParams Additional parameters like Gender and Shiny status for sprite lookup.
     * @return The graphical asset for the specified Pokémon species' battle sprite.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static FBattleRender GetSpeciesBattleSprite(FName Species, bool bBack = false,
                                                const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Retrieve the battle sprite for a given Pokémon, supporting both front and back views.
     * @param Pokemon The Pokémon to get the battle sprite for.
     * @param bBack Whether to retrieve the back sprite (true) or the front sprite (false).
     * @return The graphical asset for the specified Pokémon's battle sprite.
     */
    UFUNCTION(BlueprintCallable, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static FSoftBattleRender GetLazyPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack = false);

    /**
     * Retrieve the battle sprite for a given Pokémon species.
     * @param Species The species of Pokémon to get the battle sprite for.
     * @param bBack Whether to retrieve the back sprite (true) or the front sprite (false).
     * @param AdditionalParams Additional parameters like Gender and Shiny status for sprite lookup.
     * @return The graphical asset for the specified Pokémon species' battle sprite.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static FSoftBattleRender
    GetLazySpeciesBattleSprite(FName Species, bool bBack = false,
                               const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static FImageAsset GetPokemonIcon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @param AdditionalParams Additional params for the Pokémon
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext, AutoCreateRefTerm = AdditionalParams))
    static FImageAsset GetSpeciesIcon(FName Species,
                                      const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static FSoftImageAsset GetLazyPokemonIcon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @param AdditionalParams Additional params for the Pokémon
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext, AutoCreateRefTerm = AdditionalParams))
    static FSoftImageAsset GetLazySpeciesIcon(FName Species,
                                              const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @param bBack Is this a back sprite?
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    static FBattleRender GetTrainerSprite(const TScriptInterface<ITrainer> &Trainer, bool bBack = false);
};
