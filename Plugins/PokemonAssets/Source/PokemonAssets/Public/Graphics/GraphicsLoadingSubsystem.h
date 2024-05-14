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

/**
 * Subsystem designed to handle the loading of graphical assets into memory
 */
UCLASS()
class POKEMONASSETS_API UGraphicsLoadingSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    std::pair<UMaterialInstanceDynamic *, FVector2D> GetPokemonBattleSprite(const IPokemon &Pokemon, UObject *Outer,
                                                                            bool bBack = false) const;

    std::pair<UMaterialInstanceDynamic *, FVector2D>
    GetPokemonBattleSprite(FName Species, UObject *Outer, bool bBack = false,
                           const FPokemonAssetParams &AdditionalParams = {}) const;

    std::pair<UMaterialInstanceDynamic *, FVector2D> GetPokemonUISprite(const IPokemon &Pokemon, UObject *Outer,
                                                                        bool bBack = false) const;

    std::pair<UMaterialInstanceDynamic *, FVector2D>
    GetPokemonUISprite(FName Species, UObject *Outer, bool bBack = false,
                       const FPokemonAssetParams &AdditionalParams = {}) const;

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
    UMaterialInstanceDynamic *GetPokemonIcon(FName Species, UObject *Outer,
                                             const FPokemonAssetParams &AdditionalParams = {});

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param Trainer The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite, as well as the size of the sprite
     */
    std::pair<UMaterialInstanceDynamic *, FVector2D> GetTrainerSprite(const ITrainer &Trainer, UObject *Outer) const;

    /**
     * Get the sprite used for a trainer based on the given sprite information
     * @param TrainerType The trainer in question
     * @param Outer The owner of the created material instance
     * @return The graphical asset that displays the trainer sprite
     */
    std::pair<UMaterialInstanceDynamic *, FVector2D> GetTrainerSprite(FName TrainerType, UObject *Outer) const;

    TArray<UObject *> GetTypeIconGraphics(TConstArrayView<FName> Types) const;

    UObject *GetPokeBallIcon(FName PokeBall) const;

    UObject *GetItemIcon(FName ItemID) const;

  private:
    UPROPERTY()
    FPokemonSpriteMaterials PokemonSpriteMaterials;

    UPROPERTY()
    FTrainerSpriteMaterials TrainerSpriteMaterials;
};
