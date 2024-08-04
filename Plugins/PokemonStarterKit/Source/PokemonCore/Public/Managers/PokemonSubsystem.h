// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Pokemon/Exp/GrowthRate.h"
#include "Saving/PokemonSaveGame.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Trainers/Trainer.h"

#include "PokemonSubsystem.generated.h"

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
    UPokemonSaveGame* CreateSaveGame(TSubclassOf<UPokemonSaveGame> SaveGameClass = nullptr);

    UFUNCTION(BlueprintCallable, Category = Saving)
    void LoadSave(UPokemonSaveGame* SaveGame, bool bChangeMap = false);

    UFUNCTION(BlueprintCallable, Category = Saving)
    void AdjustPlayerTransformOnLoad(ACharacter* PlayerCharacter);

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
};
