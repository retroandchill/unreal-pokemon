// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Player/PlayerResetLocation.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Saving/PokemonSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Trainers/Trainer.h"

#include "PokemonSubsystem.generated.h"

class IStorageSystem;
class UPokemonSaveGame;
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
     * Get the Pokémon Storage System
     * @return A reference to the Pokémon storage system
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Player)
    const TScriptInterface<IStorageSystem> &GetStorageSystem() const;

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

    UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = Saving,
              meta = (DeterminesOutputType = "SaveGameClass", DynamicOutputParam = "ReturnValue"))
    UPokemonSaveGame *CreateSaveGame(TSubclassOf<UPokemonSaveGame> SaveGameClass = nullptr) const;

    UFUNCTION(BlueprintCallable, Category = Saving)
    void LoadSave(UPokemonSaveGame *SaveGame, bool bChangeMap = false);

    UFUNCTION(BlueprintCallable, Category = Saving)
    void AdjustPlayerTransformOnLoad(ACharacter *PlayerCharacter);

    /**
     * Is there a valid reset location for the player?
     * @return Is there a valid reset location for the player?
     */
    UFUNCTION(BlueprintPure, Category = Reset)
    bool IsResetLocationSet() const;

    /**
     * Perform a reset on the player, fully healing the party.
     */
    UFUNCTION(BlueprintCallable, Category = Reset)
    void PerformPlayerReset();

    /**
     * Set the player's reset location to their current location
     * @param MapName The name of the map the player should travel to on a reset.
     * @param Transform The transform that the player should reset to.
     */
    UFUNCTION(BlueprintCallable, Category = Reset, meta = (AutoCreateRefTerm = "MapName,Transform"))
    void SetPlayerResetLocation(const FString &MapName, const FTransform &Transform);

    /**
     * Set the player's reset location to their current location
     * @param PlayerCharacter The player character to draw the information from
     */
    UFUNCTION(BlueprintCallable, Category = Reset)
    void SetPlayerResetLocationAsCurrentLocation(ACharacter *PlayerCharacter);

  private:
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
     * The Pokémon storage system
     */
    UPROPERTY(BlueprintGetter = GetStorageSystem, Category = Player)
    TScriptInterface<IStorageSystem> StorageSystem;

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

    TOptional<FTransform> LoadTransform;

    TOptional<FPlayerResetLocation> PlayerResetLocation;
};
