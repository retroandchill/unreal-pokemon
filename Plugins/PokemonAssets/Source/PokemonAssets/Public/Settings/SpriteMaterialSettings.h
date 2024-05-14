// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "SpriteMaterialSettings.generated.h"

/**
 * Struct for any settings related to a Pokémon's Sprite
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FPokemonSpriteSettings {
    GENERATED_BODY()

    /**
     * The base material used to construct Pokémon sprites in battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons",
              DisplayName = "Battle Sprite Material (Battle)", meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath BattleSpritesMaterial;

    /**
     * The base material used to construct Pokémon sprites in the UI (such as the summary screen and the Pokédex)
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons", DisplayName = "Battle Sprite Material (UI)",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath UISpritesMaterial;

    /**
     * The base material used to construct Pokémon icons
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pokémon|Icons",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath IconMaterial;
};

/**
 * Struct for any settings related to a Trainer's Sprite
 */
USTRUCT(BlueprintType)
struct POKEMONASSETS_API FTrainerSpriteSettings {
    GENERATED_BODY()

    /**
     * The base material used to construct Trainer sprites in battle
     */
    UPROPERTY(EditDefaultsOnly, Config, DisplayName = "Front Sprite Material (Battle)", Category = "Trainers",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath FrontSpriteBaseMaterialBattle;

    /**
     * The base material used to construct Trainer sprites in the UI
     */
    UPROPERTY(EditDefaultsOnly, Config, DisplayName = "Front Sprite Material (UI)", Category = "Trainers",
              meta = (AllowedClasses = "MaterialInterface"))
    FSoftObjectPath FrontSpriteBaseMaterialUI;
};

/**
 * Settings related to the materials used to fetch sprites.
 */
UCLASS(Config = Game, DefaultConfig)
class POKEMONASSETS_API USpriteMaterialSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Settings for all Pokémon sprites.
     * @return Settings for all Pokémon sprites.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Sprites)
    const FPokemonSpriteSettings &GetPokemonSpriteSettings() const;

    /**
     * Settings for all Trainer sprites.
     * @return Settings for all Trainer sprites.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Sprites)
    const FTrainerSpriteSettings &GetTrainerSpriteSettings() const;

  private:
    /**
     * Settings for all Pokémon sprites.
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetPokemonSpriteSettings, Category = Sprites,
              DisplayName = "Pokémon")
    FPokemonSpriteSettings PokemonSprites;

    /**
     * Settings for all Trainer sprites.
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintGetter = GetTrainerSpriteSettings, Category = Sprites,
              DisplayName = "Trainers")
    FTrainerSpriteSettings TrainerSprites;
};
