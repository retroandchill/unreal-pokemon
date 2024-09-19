// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SpriteLoader.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GraphicsLoadingSubsystem.generated.h"

class UPaperFlipbook;
struct FTrainerSpriteSettings;
struct FPokemonSpriteSettings;
class UStaticImageRepository;
class UTextureRepository;
class ITrainer;
class IPokemon;

/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS()
class POKEMONASSETS_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetPokemonBattleSprite(const TScriptInterface<IPokemon>& Pokemon,
                                           bool bBack = false) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetSpeciesBattleSprite(FName Species, bool bBack = false,
                                           const FPokemonAssetParams& AdditionalParams = FPokemonAssetParams()) const;

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetPokemonIcon(const TScriptInterface<IPokemon>& Pokemon);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetSpeciesIcon(FName Species,
                                   const FPokemonAssetParams& AdditionalParams = FPokemonAssetParams());

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetTrainerSprite(const TScriptInterface<ITrainer>& Trainer) const;

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param TrainerType The trainer in question
     * @return The graphical asset that displays the trainer sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UPaperFlipbook* GetTrainerTypeSprite(FName TrainerType) const;

    /**
     * Get the icon graphic for a singular type.
     * @param Type The type to get the icon for.
     * @return The found icon graphic.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UObject *GetTypeIconGraphic(FName Type) const;

    /**
     * Get the icon graphics for multiple types.
     * @param Types The types to get the icon for.
     * @return The found icon graphics.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    TArray<UObject *> GetTypeIconGraphics(const TArray<FName> &Types) const;

    /**
     * Get the icon graphic for a status condition type.
     * @param Status The status to get the icon for.
     * @return The found icon graphic.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UObject *GetStatusIconGraphic(FName Status) const;

    /**
     * Get the panel graphic for a singular type.
     * @param Type The type to get the panel for.
     * @return The found panel graphic.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UObject *GetTypePanelGraphic(FName Type) const;

    /**
     * Get the icon for a Poké Ball as shown in the summary screen.
     * @param PokeBall The Poké Ball
     * @return The found icon.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UObject *GetPokeBallIcon(FName PokeBall) const;

    /**
     * Get the icon for an item for the bag screen.
     * @param ItemID The ID of the item
     * @return The item's icon.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    UObject *GetItemIcon(FName ItemID) const;
};
