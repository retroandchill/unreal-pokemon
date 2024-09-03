// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "DynamicAssetLoadingSettings.generated.h"

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
 * Settings related to the dynamic loading of assets
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Dynamic Asset Loading")
class POKEMONASSETS_API UDynamicAssetLoadingSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * Settings for all Pokémon sprites.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Sprites)
    FPokemonSpriteSettings PokemonSprites;

    /**
     * Settings for all Trainer sprites.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Sprites)
    FTrainerSpriteSettings TrainerSprites;

    /**
     * The name of the package that contains the Pokémon Icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath PokemonSpritePackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath TrainerSpritesPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath TypeIconsPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath TypePanelsPackageName;

    /**
     * The name of the package that contains the Trainer Sprite graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath StatusIconsPackageName;

    /**
     * The name of the package that contains the summary screen Poké Ball graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath SummaryBallPackageName;

    /**
     * The name of the package that contains the item icon graphics
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath ItemIconPackageName;

    /**
     * The name of the package that contains the field item effects
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath FieldItemEffectPackageName;

    /**
     * The name of the package that contains the move effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath MoveFunctionCodePackageName;

    /**
     * The name of the package that contains the battle item effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath BattleItemEffectPackageName;

    /**
     * The name of the package that contains the ability effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath AbilityBattleEffectPackageName;

    /**
     * The name of the package that contains the hold item effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath HoldItemBattleEffectPackageName;

    /**
     * The name of the package that contains the status effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath StatusEffectPackageName;

    /**
     * The prefix that is placed before a type icon when searching
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString TypeIconPrefix;

    /**
     * The prefix that is placed before a type panel when searching
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString TypePanelPrefix;

    /**
     * The prefix that is placed before a status icon when searching
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString StatusIconPrefix;

    /**
     * The prefix that is placed before a summary ball when searching
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString SummaryBallPrefix;

    /**
     * The prefix that is placed before a field item effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString FieldItemEffectPrefix;

    /**
     * The prefix that is placed before a move effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString MoveFunctionCodePrefix;

    /**
     * The prefix that is placed before a battle item when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString BattleItemEffectPrefix;

    /**
     * The prefix that is placed before an ability effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString AbilityEffectPrefix;

    /**
     * The prefix that is placed before a hold item effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString HoldItemEffectPrefix;

    /**
     * The prefix that is placed before a status item effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString StatusEffectPrefix;
};
