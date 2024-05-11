// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Breeding/PokemonGender.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "GraphicsLoadingSubsystem.generated.h"

class UStaticImageRepository;
class UTextureRepository;
class ITrainer;
class IPokemon;

USTRUCT(BlueprintType)
struct FPokemonAssetParams {
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

/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS()
class POKEMONASSETS_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    TPair<UMaterialInstanceDynamic*, FVector2D> GetPokemonBattleSprite(const IPokemon& Pokemon, UObject *Outer,  bool bBack = false);

    
    TPair<UMaterialInstanceDynamic*, FVector2D> GetPokemonBattleSprite(FName Species, UObject *Outer, bool bBack = false, const FPokemonAssetParams &AdditionalParams = {});

    /**
     * Get the icon based upon the Pokémon that was passed in.
     * @param Pokemon The Pokémon to get the icon for
     * @param Outer The owner of the created material instance
     * @return The graphical asset that this icon refers to.
     */
    UMaterialInstanceDynamic *GetPokemonIcon(const IPokemon &Pokemon, UObject *Outer);

    /**
     * Get the icon based upon the Pokémon information that was passed in.
     * @param Species The species of Pokémon to get the icon for
     * @param Outer The owner of the created material instance
     * @return The graphical asset that this icon refers to.
     */
    UMaterialInstanceDynamic *GetPokemonIcon(FName Species, UObject *Outer, const FPokemonAssetParams &AdditionalParams = {});

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    TPair<UMaterialInstanceDynamic *, FVector2D> GetTrainerSprite(const ITrainer &Trainer, UObject *Outer) const;

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param TrainerType The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite
     */
    TPair<UMaterialInstanceDynamic *, FVector2D> GetTrainerSprite(FName TrainerType, UObject *Outer) const;

    TArray<UObject*> GetTypeIconGraphics(TArrayView<FName> Types) const;

    UObject* GetPokeBallIcon(FName PokeBall) const;

  private:
    /**
     * The base material used to construct Pokémon battle sprites
     */
    UPROPERTY(EditDefaultsOnly, Category = "Icons")
    TObjectPtr<UMaterialInterface> PokemonIconsBaseMaterial;
    
    /**
     * The name of the package that contains the Pokémon Icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
    TSoftObjectPtr<UTextureRepository> PokemonSpriteRepository;
    
    /**
     * The name of the package that contains the Pokémon Icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
    TSoftObjectPtr<UTextureRepository> PokemonIconsRepository;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
    TSoftObjectPtr<UTextureRepository> TrainerSpritesRepository;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
    TSoftObjectPtr<UStaticImageRepository> TypeIconRepository;
    
    /**
     * The name of the package that contains the Summary Ball graphics
     */
    UPROPERTY(EditDefaultsOnly, Category = "Search Paths")
    TSoftObjectPtr<UStaticImageRepository> SummaryBallRepository;

    /**
     * The name of the material property for the source texture of the icons
     */
    UPROPERTY(EditDefaultsOnly, Category = "Icons")
    FName IconSourceTexturePropertyName;

    /**
     * The name of the material property for the framerate of the icon animation
     */
    UPROPERTY(EditDefaultsOnly, Category = "Icons")
    FName IconFrameRatePropertyName;

    /**
     * The base material used to construct Trainer sprites in the UI
     */
    UPROPERTY(EditDefaultsOnly, DisplayName = "Trainer Sprite Base Material (UI)", Category = "Trainers")
    TObjectPtr<UMaterialInterface> TrainerSpriteBaseMaterial;

    /**
     * The name of the material property for the source texture of the trainer sprites
     */
    UPROPERTY(EditDefaultsOnly, Category = "Trainers")
    FName TrainerSpriteSourceTexturePropertyName = TEXT("SourceTexture");
};
