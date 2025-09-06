// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "TrainerCardScreen.generated.h"

class UPlayerInfoWidget;
class UPaperFlipbookUserWidget;
class UPlayerMetadata;
class ITrainer;
class UImage;
class UDisplayText;

/**
 * The screen used to bring up the trainer card.
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UTrainerCardScreen : public UScreen
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UTrainerCardScreen *AddTrainerCardScreenToStack(const UObject *WorldContextObject);

    /**
     * Retrieve the player's trainer information.
     *
     * This function provides access to the current trainer associated with the player.
     * It is marked as BlueprintPure and BlueprintInternalUseOnly.
     *
     * @return A constant reference to the TScriptInterface of the ITrainer interface representing the player's trainer.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    const TScriptInterface<ITrainer> &GetPlayerTrainer() const
    {
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
    UPlayerMetadata *GetPlayerMetadata() const
    {
        return PlayerMetadata;
    }

  protected:
    void NativeConstruct() override;

    /**
     * Event triggered when the player's trainer and metadata are set.
     *
     * This event is designed to be implemented in Blueprints. It allows for custom actions
     * to be performed whenever the player's trainer and metadata are updated.
     *
     * @param Trainer A constant reference to the TScriptInterface of the ITrainer interface representing the new
     * trainer assigned to the player.
     * @param Metadata A pointer to the UPlayerMetadata object representing the new metadata assigned to the player.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = Content)
    void OnPlayerInfoSet(const TScriptInterface<ITrainer> &Trainer, UPlayerMetadata *Metadata);

  private:
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

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UTrainerCardScreen)