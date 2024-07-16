// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

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
     * The path to the repository data asset used to get the information for move selection buttons.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Asset Loaders", meta = (AllowedClasses = "StaticImageRepository"))
    FSoftObjectPath TypePanelRepository;

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

/**
 * Contains the paths of dynamic assets within the system
 */
USTRUCT(BlueprintType)
struct FDynamicAssetPaths {
    GENERATED_BODY()

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
     * The name of the package that contains the move effect Blueprints
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Search Paths", meta = (ContentDir))
    FDirectoryPath MoveFunctionCodePackageName;

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
     * The prefix that is placed before a move effect when looking it up
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Prefixes")
    FString MoveFunctionCodePrefix;

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
};

/**
 * The information for a stage of stat alteration.
 */
USTRUCT(BlueprintType)
struct FStatStageInfo {
    GENERATED_BODY()

    /**
     * The amount of the stat will be multiplied by for a positive stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float PositiveStatMultiplier;

    /**
     * The amount of the stat will be multiplied by for a negative stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float NegativeStatMultiplier;

    /**
     * The amount of the accuracy/evasion will be multiplied by for a positive stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, DisplayName = "Positive Accuracy/Evasion Multiplier",
              Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float PositiveAccEvaMultiplier;

    /**
     * The amount of the accuracy/evasion will be multiplied by for a negative stage
     */
    UPROPERTY(EditDefaultsOnly, Config, BlueprintReadOnly, DisplayName = "Negative Accuracy/Evasion Multiplier",
              Category = "Stats", meta = (UIMin = 0, ClampMin = 0))
    float NegativeAccEvaMultiplier;
};

/**
 * Struct that contains information for Gameplay Tags that are applied when HP dips below a certain amount.
 */
USTRUCT(BlueprintType, DisplayName = "HP State Tag")
struct POKEMONSETTINGS_API FHPStateTag {
    GENERATED_BODY()

    /**
     * The threshold to apply the tag if HP is under.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|HP",
              meta = (UIMin = 0.f, ClampMin = 0.f, UIMax = 1.f, ClampMax = 1.f))
    float Threshold;

    /**
     * The tag to apply
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|HP")
    FGameplayTag Tag;
};

USTRUCT(BlueprintType)
struct POKEMONSETTINGS_API FDefaultScreens {
    GENERATED_BODY()

    /**
     * The screen displayed when showing a messages
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (MetaClass = "TextDisplayScreen"))
    FSoftClassPath TextScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (MetaClass = "PokemonSelectScreen"))
    FSoftClassPath PartyScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (MetaClass = "BagScreen"))
    FSoftClassPath BagScreenClass;

    /**
     * The screen displayed when showing the party screen
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI, meta = (MetaClass = "MoveForgetScreen"))
    FSoftClassPath MoveForgetScreenClass;
};