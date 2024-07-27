// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "SpriteMaterials.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GraphicsLoadingSubsystem.generated.h"

struct FTrainerSpriteSettings;
struct FPokemonSpriteSettings;
class UStaticImageRepository;
class UTextureRepository;
class ITrainer;
class IPokemon;

USTRUCT(BlueprintType)
struct POKEMONASSETS_API FPokemonAssetParams {
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites, meta = (UIMin = 0, ClampMin = 0))
    int32 Form = 0;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    EPokemonGender Gender = EPokemonGender::Male;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShiny = false;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Sprites)
    bool bShadow = false;
};

USTRUCT(BlueprintType)
struct POKEMONASSETS_API FMaterialInstanceWithSize {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    TObjectPtr<UMaterialInstanceDynamic> Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Graphics")
    FVector2D Size;
};

/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS()
class POKEMONASSETS_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetPokemonBattleSprite(const TScriptInterface<IPokemon> &Pokemon, UObject *Outer,
                                                     bool bBack = false) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize
    GetSpeciesBattleSprite(FName Species, UObject *Outer, bool bBack = false,
                           const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams()) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetPokemonUISprite(const TScriptInterface<IPokemon> &Pokemon, UObject *Outer,
                                                 bool bBack = false) const;

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize
    GetSpeciesUISprite(FName Species, UObject *Outer, bool bBack = false,
                       const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams()) const;

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @param Outer The owner of the created material instance
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetPokemonIcon(const TScriptInterface<IPokemon> &Pokemon, UObject *Outer);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @param Outer The owner of the created material instance
     * @return The graphical asset that this icon refers to.
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetSpeciesIcon(FName Species, UObject *Outer,
                                             const FPokemonAssetParams &AdditionalParams = FPokemonAssetParams());

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetTrainerSprite(const TScriptInterface<ITrainer> &Trainer, UObject *Outer) const;

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param TrainerType The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite
     */
    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Graphics|Pokémon")
    FMaterialInstanceWithSize GetTrainerTypeSprite(FName TrainerType, UObject *Outer) const;

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

  private:
    /**
     * The materials for a Pokémon sprites
     */
    UPROPERTY()
    FPokemonSpriteMaterials PokemonSpriteMaterials;

    /**
     * The materials for trainer sprites.
     */
    UPROPERTY()
    FTrainerSpriteMaterials TrainerSpriteMaterials;
};
