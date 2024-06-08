﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#include "SettingsStructs.generated.h"

/**
 * Meta info about a pocket in the bag.
 */
USTRUCT(BlueprintType)
struct FPocketInfo {
    GENERATED_BODY()

    /**
     * The name of the pocket as displayed to the player.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag")
    FText DisplayName;

    /**
     * The maximum number of items that can be held within a given pocket.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag", meta = (UIMin = 1, ClampMin = 1))
    TOptional<int32> MaxPocketSize;

    /**
     * Should the pocket be automatically sorted when items are added
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bag")
    bool bAutoSort;
};

/**
 * Struct for any settings related to a Pokémon's Sprite
 */
USTRUCT(BlueprintType)
struct POKEMONSETTINGS_API FPokemonSpriteSettings {
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
struct POKEMONSETTINGS_API FTrainerSpriteSettings {
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
 * The struct of all sprite repositories
 */
USTRUCT(BlueprintType)
struct FSpriteRepositories {
    GENERATED_BODY()

    /**
     * The path to the repository data asset used to get the icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "TextureRepository"))
    FSoftObjectPath PokemonSpriteRepository;

    /**
     * The path to the repository data asset used to get the trainers.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "TextureRepository"))
    FSoftObjectPath TrainerFrontSpriteRepository;

    /**
     * The path to the repository data asset used to get the type icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath TypeIconRepository;

    /**
     * The path to the repository data asset used to get the status icons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath StatusIconRepository;

    /**
     * The path to the repository data asset used to get the Poké Ball icons on the summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath SummaryBallRepository;

    /**
     * The path to the repository data asset used to get the item icons used in the bag and summary screen.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath ItemIconRepository;
};