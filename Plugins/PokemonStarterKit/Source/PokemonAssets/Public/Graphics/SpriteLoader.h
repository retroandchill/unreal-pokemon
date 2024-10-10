// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Pokemon/Breeding/PokemonGender.h"

#include "SpriteLoader.generated.h"

class ITrainer;
class IPokemon;
class UPaperFlipbook;

USTRUCT(BlueprintType)
struct POKEMONASSETS_API FPokemonAssetParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites, meta = (UIMin = 0, ClampMin = 0))
    FName Form = NAME_None;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    EPokemonGender Gender = EPokemonGender::Male;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShiny = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShadow = false;
};

/**
 * Blueprint Function library for loading sprites dynamically.
 */
UCLASS()
class POKEMONASSETS_API USpriteLoader : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static UPaperFlipbook *GetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack = false);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static UPaperFlipbook *GetSpeciesBattleSprite(FName Species, bool bBack = false,
                                                  const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    UFUNCTION(BlueprintCallable, Category = "Sprites|Pokémon", meta = (CallableWithoutWorldContext))
    static TSoftObjectPtr<UPaperFlipbook> GetLazyPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, bool bBack = false);

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static TSoftObjectPtr<UPaperFlipbook> GetLazySpeciesBattleSprite(FName Species, bool bBack = false,
                                                  const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static UPaperFlipbook *GetPokemonIcon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static UPaperFlipbook *GetSpeciesIcon(FName Species,
                                          const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static TSoftObjectPtr<UPaperFlipbook> GetLazyPokemonIcon(const TScriptInterface<IPokemon> &Pokemon);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon",
              meta = (CallableWithoutWorldContext))
    static TSoftObjectPtr<UPaperFlipbook> GetLazySpeciesIcon(FName Species,
                                          const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    static TArray<FString> CreatePokemonSpriteResolutionList(FName Species, const FPokemonAssetParams &Params,
                                                             FStringView Subfolder);

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @param bBack
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    static UPaperFlipbook *GetTrainerSprite(const TScriptInterface<ITrainer> &Trainer, bool bBack = false);
};
