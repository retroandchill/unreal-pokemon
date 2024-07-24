// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"

#include "PokemonDataSettings.generated.h"

/**
 * Meta info about a pocket in the bag.
 */
USTRUCT(BlueprintType)
struct POKEMONDATA_API FPocketInfo {
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
 * Data related to the Pokémon core data tables used by the game
 */
UCLASS(Config = Game, DefaultConfig, DisplayName = "Pokémon Data")
class POKEMONDATA_API UPokemonDataSettings : public UDeveloperSettings {
    GENERATED_BODY()

  public:
    /**
     * The maximum amount of money the player can have.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Trainers|Player",
              meta = (UIMin = 1, ClampMin = 1))
    int32 MaxMoney = 999999;

    /**
     * The maximum length, in characters, that the player's name can be.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Trainers|Player",
              meta = (UIMin = 1, ClampMin = 1))
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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Party and Pokémon storage",
              meta = (UIMin = 1, ClampMin = 1))
    int32 MaxPartySize = 6;

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
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Abilities)
    FText NoAbilityName;

    /**
     * The text displayed for the description when a Pokémon has no ability.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = Abilities)
    FText NoAbilityDescription;

    /**
     * The set of data tables used by the game.
     */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Config, Category = "Data", meta = (AllowedClasses = "DataTable"))
    TArray<FSoftObjectPath> DataTables;
};
