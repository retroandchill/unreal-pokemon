// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Trainers/Trainer.h"

#include "PokemonSubsystem.generated.h"

class IBag;
class UPlayerMetadata;
/**
 * Subsystem for interfacing with the Pokémon data. Has a backdoor static pointer for non-UObject singleton access.
 */
UCLASS()
class POKEMONCORE_API UPokemonSubsystem : public UGameInstanceSubsystem {
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    /**
     * Get the instance of this subsystem from the provided context.
     * @param WorldContext The object used to get the game instance.
     * @return A reference to this subsystem
     */
    static UPokemonSubsystem &GetInstance(const UObject *WorldContext);

    /**
     * Check if this subsystem exists or not.
     * @param WorldContext The object used to get the game instance.
     * @return Does this subsystem exist?
     */
    static bool Exists(const UObject *WorldContext);

    /**
     * Start a brand new game for the player
     */
    UFUNCTION(BlueprintCallable, Category = "Playthrough")
    void StartNewGame();

    /**
     * The the ID of the HP stat
     * @return The stat used referring to a Pokémon's HP
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Display Names")
    FName GetHPStat() const;

    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     * @return The maximum number of Pokémon a trainer can carry at any given time
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Party)
    int32 GetMaxPartySize() const;

    /**
     * Get the player trainer
     * @return A reference to the player trainer character
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Player)
    const TScriptInterface<ITrainer> &GetPlayer() const;

    /**
     * Get the player's bag
     * @return A reference to the player's bag
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Player)
    const TScriptInterface<IBag> &GetBag() const;

    /**
     * Get the metadata about the current player
     * @return The metadata about the current player
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Player)
    UPlayerMetadata *GetPlayerMetadata() const;

    /**
     * Get the growth rate for the given name
     * @param GrowthRate The growth rate to look up
     * @return The found GrowthRate
     */
    const Exp::IGrowthRate &GetGrowthRate(FName GrowthRate) const;

    /**
     * Get the name of the player's current location
     * @return The player's current location
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Maps|Display")
    FText GetCurrentLocation() const;

    /**
     * Set the name of the player's current location
     * @param LocationName The player's current location
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "Maps|Display")
    void SetCurrentLocation(const FText &LocationName);

  private:
    /**
     * The stat used referring to a Pokémon's HP
     */
    UPROPERTY(BlueprintGetter = GetHPStat, DisplayName = "HP Stat", Category = "Display Names")
    FName HPStat;

    /**
     * The maximum number of Pokémon a trainer can carry at any given time
     */
    UPROPERTY(BlueprintGetter = GetMaxPartySize, Category = Party)
    int32 MaxPartySize;

    /**
     * The trainer that represents the player character
     */
    UPROPERTY(BlueprintGetter = GetPlayer, Category = Player)
    TScriptInterface<ITrainer> Player;

    /**
     * The player's bag
     */
    UPROPERTY(BlueprintGetter = GetBag, Category = Player)
    TScriptInterface<IBag> Bag;

    /**
     * The metadata about the player
     */
    UPROPERTY(BlueprintGetter = GetPlayerMetadata, Category = Player)
    TObjectPtr<UPlayerMetadata> PlayerMetadata;

    /**
     * The list of GrowthRate objects used by the game
     */
    TMap<FName, TUniquePtr<Exp::IGrowthRate>> GrowthRates;

    /**
     * The player's current location in the world.
     */
    UPROPERTY(BlueprintGetter = GetCurrentLocation, BlueprintSetter = SetCurrentLocation)
    FText CurrentLocation;
};
