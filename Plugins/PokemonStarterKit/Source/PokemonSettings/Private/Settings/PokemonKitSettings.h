// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Settings/SettingsStructs.h"

#include "PokemonKitSettings.generated.h"

namespace Pokemon {
class FBaseSettings;
}

/**
 * The actual internal settings class of the starter kit.
 */
UCLASS(NotBlueprintable, NotBlueprintType, Config = Game, DefaultConfig, DisplayName = "Pokémon Starter Kit")
class POKEMONSETTINGS_API UPokemonKitSettings : public UDeveloperSettings {
    GENERATED_BODY()

  protected:
#if WITH_EDITOR
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;
#endif

public:

    /**
     * The maximum amount of money the player can have.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Trainers|Player", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxMoney = 999999;

    /**
     * The maximum length, in characters, that the player's name can be.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Trainers|Player", meta = (UIMin = 1, ClampMin = 1))
    uint8 MaxPlayerNameSize = 12;

    /**
     * The maximum level Pokémon can reach.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaximumLevel = 0;

    /**
     * The level of newly hatched Pokémon.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 EggLevel = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, DisplayName = "HP Stat", Category = "Pokémon",
              meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName HPStat = TEXT("HP");

    /**
     * The maximum number of abilities a Pokémon can have.
     * <p>If a Pokémon has less abilities here, the last ability in the list is duplicated.</p>
     * <p>If the list has more than the required amount, then the list is truncated, and a warning is emitted.</p>
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxDefaultAbilities = 2;

    /**
     * The maximum number of moves a Pokémon can know.
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxMoves = 4;

    /**
     * The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Pokémon",
              meta = (UIMin = 0, ClampMin = 0, UIMax = 65536, ClampMax = 65536))
    int32 ShinyPokemonChance = 0;

    /**
     * The default Poké Ball a Pokémon is housed in
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Pokémon",
              meta = (GetOptions = "PokemonData.ItemHelper.GetPokeBallNames"))
    FName DefaultPokeBall = TEXT("POKEBALL");

    /**
     * The maximum number of Pokémon that can be in the party.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Party and Pokémon storage", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxPartySize = 0;

    /**
     * Map of pocket id numbers found in the imported PBS files to their actual names.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Bag")
    TMap<uint8, FName> PocketNames;

    /**
     * Information about the various Bag Pockets.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Bag",
              meta = (GetKeyOptions = "PokemonData.ItemHelper.GetPocketNames"))
    TMap<FName, FPocketInfo> PocketInfo;

    /**
     * The maximum number of items each slot in the Bag can hold.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Bag", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxItemsPerSlot = 999;

    /**
     * The text displayed for the name when a Pokémon has no ability.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI)
    FText NoAbilityName;

    /**
     * The text displayed for the description when a Pokémon has no ability.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = UI)
    FText NoAbilityDescription;

    /**
     * The information about how stat stages are altered
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle)
    TArray<FStatStageInfo> StatStages = {
        {3.f / 2.f, 2.f / 3.f, 4.f / 3.f, 3.f / 4.f},
        {4.f / 2.f, 2.f / 4.f, 5.f / 3.f, 3.f / 5.f},
        {5.f / 2.f, 2.f / 5.f, 6.f / 3.f, 3.f / 6.f},
        {6.f / 2.f, 2.f / 6.f, 7.f / 3.f, 3.f / 7.f},
        {7.f / 2.f, 2.f / 7.f, 8.f / 3.f, 3.f / 8.f},
        {8.f / 2.f, 2.f / 8.f, 9.f / 3.f, 3.f / 9.f},
    };

    /**
     * The default ability used for using moves
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle, meta = (MetaClass="BattleMoveFunctionCode"))
    FSoftClassPath DefaultMoveAbility;

    /**
     * The default level that is streamed in when starting a battle
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle, meta = (AllowedClasses = "/Script/Engine.World"))
    FSoftObjectPath DefaultBattleScene;

    /**
     * The offset of the streamed in battle scene
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = Battle)
    FVector BattleSceneOffset = FVector(0, 0, -5000);

    /**
     * The tags for the HP state of the battler
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, DisplayName = "HP State Tags", Category = Battle)
    TArray<FHPStateTag> HPStateTags;

    /**
     * The damage multiplier for a critical hit
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle)
    float CriticalHitMultiplier = 1.5f;
    
    /**
     * The critical hit ratios in battle expressed as 1 / the specified value for each number of boosts.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Battle, meta = (UIMin = 1, ClampMin = 1))
    TArray<int32> CriticalHitRatios = { 24, 8, 2, 1 };

    /**
     * The set of data tables used by the game.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Data", meta = (AllowedClasses = "DataTable"))
    TArray<FSoftObjectPath> DataTables;

    /**
     * The class for the Pokémon utilities
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Untility Classes", meta = (MustImplement = "PokemonUtilities"))
    FSoftClassPath PokemonUtilitiesClass;

    /**
     * The blueprint class that contains the item utilities information
     */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Config, Category = "Utility Classes", meta = (MustImplement = "ItemUtilities"))
    FSoftClassPath ItemUtilitiesClass;

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
     * The repositories for all the different sprites
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Sprites)
    FSpriteRepositories SpriteRepositories;

    /**
     * The paths used to dynamically load assets in the game.
     * <p><b>NOTE:</b> These paths need to be configured as additional directories to cook otherwise they won't be
     * available in a packaged build</p>
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Assets)
    FDynamicAssetPaths DynamicAssetPaths;

    /**
     * The paths to the UI classes used to load the screens
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Assets)
    FDefaultScreens DefaultScreenPaths;
};
