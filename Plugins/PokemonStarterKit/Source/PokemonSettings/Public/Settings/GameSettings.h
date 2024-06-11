// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "SettingsStructs.h"
#include "UObject/Interface.h"
#include "GameSettings.generated.h"

struct FRivalName;
// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class UGameSettings : public UInterface {
    GENERATED_BODY()
};

/**
 * Interface that wraps around the game settings.
 */
class POKEMONSETTINGS_API IGameSettings {
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

    /**
     * The maximum amount of money the player can have.
     * @return The maximum amount of money the player can have.
     */
    UFUNCTION(BlueprintCallable, Category = "Trainers|Player")
    virtual int32 GetMaxMoney() const = 0;

    /**
     * The maximum length, in characters, that the player's name can be.
     * @return The maximum length, in characters, that the player's name can be.
     */
    UFUNCTION(BlueprintCallable, Category = "Trainers|Player")
    virtual uint8 GetMaxPlayerNameSize() const = 0;

    /**
     * The maximum level Pokémon can reach.
     * @return The maximum level Pokémon can reach.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual int32 GetMaximumLevel() const = 0;

    /**
     * The level of newly hatched Pokémon.
     * @return The level of newly hatched Pokémon.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual int32 GetEggLevel() const = 0;

    /**
     * The the ID of the HP stat
     * @return The stat used referring to a Pokémon's HP
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual FName GetHPStat() const = 0;

    /**
     * The maximum number of abilities a Pokémon can have.
     * @return The maximum number of abilities a Pokémon can have.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual int32 GetMaxDefaultAbilities() const = 0;

    /**
     * The maximum number of moves a Pokémon can know.
     * @return The maximum number of moves a Pokémon can know.
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual int32 GetMaxMoves() const = 0;

    /**
     * The odds of a newly generated Pokémon being shiny (out of 65536).
     * @return The odds of a newly generated Pokémon being shiny (out of 65536).
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual int32 GetShinyPokemonChance() const = 0;

    /**
     * The default Poké Ball a Pokémon is housed in
     * @return The default Poké Ball a Pokémon is housed in
     */
    UFUNCTION(BlueprintCallable, Category = "Pokémon")
    virtual FName GetDefaultPokeBall() const = 0;

    /**
     * The maximum number of Pokémon that can be in the party.
     * @return The maximum number of Pokémon that can be in the party.
     */
    UFUNCTION(BlueprintCallable, Category = "Party and Pokémon storage")
    virtual int32 GetMaxPartySize() const = 0;

    /**
     * Map of pocket id numbers found in the imported PBS files to their actual names.
     * @return Map of pocket id numbers found in the imported PBS files to their actual names.
     */
     UFUNCTION(BlueprintCallable,  Category = "Bag")
     virtual const TMap<uint8, FName> &GetPocketNames() const = 0;

    /**
     * Information about the various Bag Pockets.
     */
     UFUNCTION(BlueprintCallable,  Category = "Bag")
     virtual const TMap<FName, FPocketInfo> &GetPocketInfo() const = 0;

    /**
     * The maximum number of items each slot in the Bag can hold.
     * @return The maximum number of items each slot in the Bag can hold.
     */
    UFUNCTION(BlueprintCallable, Category = "Bag")
    virtual int32 GetMaxItemsPerSlot() const = 0;

    /**
     * The text displayed for the name when a Pokémon has no ability.
     * @return The text displayed for the name when a Pokémon has no ability.
     */
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual const FText &GetNoAbilityName() const = 0;

    /**
     * The text displayed for the description when a Pokémon has no ability.
     * @return The text displayed for the description when a Pokémon has no ability.
     */
    UFUNCTION(BlueprintCallable, Category = UI)
    virtual const FText &GetNoAbilityDescription() const = 0;

    /**
     * The move used when a Pokémon has no other move options to use
     * @return The move used when a Pokémon has no other move options to use
     */
    UFUNCTION(BlueprintCallable, Category = Battle)
    virtual FName GetMoveOfLastResort() const = 0;

};

/**
 * Function library used to retrieve the settings used by the game itself.
 */
UCLASS()
class POKEMONSETTINGS_API UGameSettingsFunctionLibrary : public UBlueprintFunctionLibrary {
    GENERATED_BODY()

public:
    /**
     * Get the game settings interface.
     * @return The wrapped settings interface
     */
    UFUNCTION(BlueprintPure, Category = "Settings")
    static const TScriptInterface<IGameSettings> &GetGameSettings();
};