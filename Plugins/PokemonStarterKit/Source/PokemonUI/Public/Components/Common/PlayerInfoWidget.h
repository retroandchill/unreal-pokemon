// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "PlayerInfoWidget.generated.h"

class UPlayerMetadata;
class ITrainer;

/**
 * UPlayerInfoWidget displays information related to the player's trainer and metadata.
 *
 * This class extends UCommonUserWidget and provides both retrieval and assignment functionalities
 * for player trainer and metadata information. It also includes Blueprint events for handling updates.
 */
UCLASS(Abstract)
class POKEMONUI_API UPlayerInfoWidget : public UCommonUserWidget {
    GENERATED_BODY()

public:
    /**
     * Retrieve the player's trainer information.
     *
     * This function provides access to the current trainer associated with the player.
     * It is marked as BlueprintPure and BlueprintInternalUseOnly.
     *
     * @return A constant reference to the TScriptInterface of the ITrainer interface representing the player's trainer.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<ITrainer> &GetPlayerTrainer() const {
        return PlayerTrainer;
    }

    /**
     * Retrieve the player's metadata.
     *
     * Provides access to the current metadata associated with the player.
     * It is marked as BlueprintPure and BlueprintInternalUseOnly.
     *
     * @return A pointer to the UPlayerMetadata associated with the player.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UPlayerMetadata *GetPlayerMetadata() const {
        return PlayerMetadata;
    }

    /**
     * Set the player's trainer information and metadata.
     *
     * This function assigns a new trainer and accompanying metadata to the player,
     * and handles the subsequent player info updates.
     *
     * @param NewTrainer A constant reference to the TScriptInterface of the ITrainer interface representing the new trainer to be assigned.
     * @param NewPlayerMetadata A pointer to the UPlayerMetadata object representing the new player metadata to be assigned.
     */
    UFUNCTION(BlueprintCallable, Category = Content)
    void SetPlayerInfo(const TScriptInterface<ITrainer> &NewTrainer, UPlayerMetadata *NewPlayerMetadata);
    
protected:
    /**
     * Event triggered when the player's trainer and metadata are set.
     *
     * This event is designed to be implemented in Blueprints. It allows for custom actions
     * to be performed whenever the player's trainer and metadata are updated.
     *
     * @param NewTrainer A constant reference to the TScriptInterface of the ITrainer interface representing the new trainer assigned to the player.
     * @param NewPlayerMetadata A pointer to the UPlayerMetadata object representing the new metadata assigned to the player.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void OnPlayerInfoSet(const TScriptInterface<ITrainer> &NewTrainer, UPlayerMetadata *NewPlayerMetadata);

    /**
     * Set the display text for the time
     * 
     * @param Playtime The new playtime
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void SetPlayerTimeInfo(float Playtime);

private:
    void HandlePlayerInfoSet(const TScriptInterface<ITrainer> &NewTrainer, UPlayerMetadata *NewPlayerMetadata);
    
    /**
      * The player trainer we're displaying information for.
      */
    UPROPERTY(BlueprintGetter = GetPlayerTrainer, Category = Content)
    TScriptInterface<ITrainer> PlayerTrainer;

    /**
     * The metadata about the player we're looking at.
     */
    UPROPERTY(BlueprintGetter = GetPlayerMetadata, Category = Content)
    TObjectPtr<UPlayerMetadata> PlayerMetadata;

};