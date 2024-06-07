﻿// "Unreal Pokémon" created by Retro & Chill.

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
    
private:
    friend Pokemon::FBaseSettings;
    
    /**
     * The maximum amount of money the player can have.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Trainers|Player", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxMoney = 999999;

    /**
     * The maximum length, in characters, that the player's name can be.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Trainers|Player", meta = (UIMin = 1, ClampMin = 1))
    uint8 MaxPlayerNameSize = 12;
    
    /**
     * The maximum level Pokémon can reach.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaximumLevel = 0;

    /**
     * The level of newly hatched Pokémon.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 EggLevel = 0;

    UPROPERTY(EditAnywhere, Config, DisplayName = "HP Stat", Category = "Pokémon",
        meta = (GetOptions = "PokemonData.StatHelper.GetMainStatNames"))
    FName HPStat = TEXT("HP");

    /**
     * The maximum number of abilities a Pokémon can have.
     * <p>If a Pokémon has less abilities here, the last ability in the list is duplicated.</p>
     * <p>If the list has more than the required amount, then the list is truncated, and a warning is emitted.</p>
     */
    UPROPERTY(EditAnywhere, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxDefaultAbilities = 2;

    /**
     * The maximum number of moves a Pokémon can know.
     */
    UPROPERTY(EditAnywhere, Config, Category = "Pokémon", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxMoves = 4;

    /**
     * The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Pokémon",
        meta = (UIMin = 0, ClampMin = 0, UIMax = 65536, ClampMax = 65536))
    int32 ShinyPokemonChance = 0;

    /**
     * The default Poké Ball a Pokémon is housed in
     */
    UPROPERTY(EditAnywhere, Config, Category = "Pokémon",
              meta = (GetOptions = "PokemonData.ItemHelper.GetPokeBallNames"))
    FName DefaultPokeBall = TEXT("POKEBALL");

    /**
     * The maximum number of Pokémon that can be in the party.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Party and Pokémon storage", meta = (UIMin = 1, ClampMin = 1))
    int32 MaxPartySize = 0;

    /**
     * Map of pocket id numbers found in the imported PBS files to their actual names.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Bag")
    TMap<uint8, FName> PocketNames;

    /**
     * Information about the various Bag Pockets.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Bag",
        meta = (GetKeyOptions = "PokemonData.ItemHelper.GetPocketNames"))
    TMap<FName, FPocketInfo> PocketInfo;

    /**
     * The maximum number of items each slot in the Bag can hold.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Bag", meta = (UIMin = 1, UIMax = 1))
    int32 MaxItemsPerSlot = 0;

    /**
     * The set of data tables used by the game.
     */
    UPROPERTY(EditDefaultsOnly, Config, Category = "Data Tables", meta = (AllowedClasses = "DataTable"))
    TArray<FSoftObjectPath> DataTables;
};
