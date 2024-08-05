// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "PlayerMetadata.generated.h"

/**
 * Delegate for when the player
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTimeUpdate, float, NewPlaytime);

/**
 * Various playthrough related metadata for the player character, such as playtime.
 */
UCLASS()
class POKEMONCORE_API UPlayerMetadata : public UObject, public FTickableGameObject {
    GENERATED_BODY()

  public:
    void Tick(float DeltaTime) override;
    TStatId GetStatId() const override;
    UWorld *GetTickableGameObjectWorld() const override;

    /**
     * Get the datetime the game was started on.
     * @return The datetime the player started the game on.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Metadata")
    const FDateTime &GetStartDate() const;

    /**
     * Retrieve the player's total playtime
     * @return The total amount of time (in seconds) that the game has been played
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "Player|Metadata")
    float GetTotalPlaytime() const;

    /**
     * Retrieves the delegate for when the time is updated
     * @return Called when the player's playtime is updated. Should be emitted about every minute.
     */
    FOnTimeUpdate &GetOnTimeUpdated();

  private:
    /**
     * Start a brand new game
     */
    void StartNewGame();

    /**
     * The datetime the player started the game on.
     */
    UPROPERTY(BlueprintGetter = GetStartDate, SaveGame, Category = "Player|Metadata")
    FDateTime StartDate;

    /**
     * The total amount of time (in seconds) that the game has been played
     */
    UPROPERTY(BlueprintGetter = GetTotalPlaytime, SaveGame, Category = "Player|Metadata")
    float TotalPlaytime;

    /**
     * The time since the last update was called
     */
    float LastUpdated = 0;

    /**
     * Called when the player's playtime is updated. Should be emitted about every minute.
     */
    UPROPERTY(BlueprintAssignable, Category = "Player|Metadata")
    FOnTimeUpdate OnTimeUpdated;

    friend class UPokemonSubsystem;
};
