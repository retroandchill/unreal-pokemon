// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "SpriteMaterials.generated.h"

struct FPokemonSpriteSettings;
struct FTrainerSpriteSettings;
/**
 * Materials for all Pokémon sprites
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FPokemonSpriteMaterials {
    GENERATED_BODY()

    /**
     * The base material used to construct Pokémon sprites in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons",
              DisplayName = "Battle Sprite Material (Battle)", meta = (AllowedClasses = "MaterialInterface"))
    TSoftObjectPtr<UMaterialInterface> BattleSpritesMaterial;

    /**
     * The base material used to construct Pokémon sprites in the UI (such as the summary screen and the Pokédex)
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons", DisplayName = "Battle Sprite Material (UI)",
              meta = (AllowedClasses = "MaterialInterface"))
    TSoftObjectPtr<UMaterialInterface> UISpritesMaterial;

    /**
     * The base material used to construct Pokémon icons
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons",
              meta = (AllowedClasses = "MaterialInterface"))
    TSoftObjectPtr<UMaterialInterface> IconMaterial;

    /**
     * Construct the default version of the struct.
     */
    FPokemonSpriteMaterials();

    /**
     * Construct a new instance of this object from the supplied settings.
     * @param Settings The settings to copy the information from
     */
    explicit FPokemonSpriteMaterials(const FPokemonSpriteSettings &Settings);

    /**
     * Copy over the reference form the supplied settings and assign them into this object
     * @param Settings The settings to copy the information from
     * @return A reference to the target object
     */
    FPokemonSpriteMaterials &operator=(const FPokemonSpriteSettings &Settings);
};

/**
 * Materials for all Trainer sprites
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FTrainerSpriteMaterials {
    GENERATED_BODY()

    /**
     * The base material used to construct Trainer sprites in battle
     */
    UPROPERTY(EditDefaultsOnly, Config, DisplayName = "Front Sprite Material (Battle)", Category = "Trainers",
              meta = (AllowedClasses = "MaterialInterface"))
    TSoftObjectPtr<UMaterialInterface> FrontSpriteBaseMaterialBattle;

    /**
     * The base material used to construct Trainer sprites in the UI
     */
    UPROPERTY(EditDefaultsOnly, Config, DisplayName = "Front Sprite Material (UI)", Category = "Trainers",
              meta = (AllowedClasses = "MaterialInterface"))
    TSoftObjectPtr<UMaterialInterface> FrontSpriteBaseMaterialUI;

    /**
     * Construct the default version of the struct.
     */
    FTrainerSpriteMaterials();

    /**
     * Construct a new instance of this object from the supplied settings.
     * @param Settings The settings to copy the information from
     */
    explicit FTrainerSpriteMaterials(const FTrainerSpriteSettings &Settings);

    /**
     * Copy over the reference form the supplied settings and assign them into this object
     * @param Settings The settings to copy the information from
     * @return A reference to the target object
     */
    FTrainerSpriteMaterials &operator=(const FTrainerSpriteSettings &Settings);
};