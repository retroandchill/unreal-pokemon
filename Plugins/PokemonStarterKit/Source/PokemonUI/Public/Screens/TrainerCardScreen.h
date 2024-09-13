// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "TrainerCardScreen.generated.h"

class UPaperFlipbookUserWidget;
class UPlayerMetadata;
class ITrainer;
class UImage;
class UDisplayText;

/**
 * The screen used to bring up the trainer card.
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UTrainerCardScreen : public UScreen {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;

  private:
    /**
     * Set the graphic used for the trainer's sprite
     */
    void SetTrainerSprite();

    /**
     * Set the textual information for the player
     */
    void SetTrainerInfo();

    /**
     * Set the display text for the time
     * @param Playtime The new playtime
     */
    UFUNCTION()
    void SetPlayerTimeInfo(float Playtime);

    /**
     * The trainer we're displaying information for
     */
    UPROPERTY()
    TScriptInterface<ITrainer> Trainer;

    /**
     * The metadata about the player we're looking at
     */
    UPROPERTY()
    TObjectPtr<UPlayerMetadata> PlayerMetadata;

    /**
     * The image used for the player's sprite
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UPaperFlipbookUserWidget> TrainerImage;

    /**
     * The text used to display the player's name
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> TrainerNameText;

    /**
     * The text used to display the player's ID number
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> IDText;

    /**
     * The text used to display the player's current playtime
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> TimeText;

    /**
     * The text used to display the player's current Pokédex progress
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> PokedexText;

    /**
     * The text used to display the player's current money
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> MoneyText;

    /**
     * The text used to display the date the player started their adventure on
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> AdventureStartedText;

    /**
     * The text used to display the date the player first entered the hall of fame
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> FirstHallOfFameText;

    /**
     * The length of the player's ID number
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin = 1))
    int32 IdNumberLength = 6;
};
