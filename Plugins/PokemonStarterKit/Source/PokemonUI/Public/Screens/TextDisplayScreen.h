// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Screens/Screen.h"

#include "TextDisplayScreen.generated.h"

class UCommand;
class UCommandWindow;
class UMessageWindow;

/**
 * Handle advancing to the next message
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNextMessage);

/**
 * Handle advancing to the next message
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChoiceMade, int32, Index, FName, Choice);

/**
 * Screen for displaying text to the player
 */
UCLASS(Blueprintable, Abstract)
class POKEMONUI_API UTextDisplayScreen : public UScreen {
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = Screens, meta = (WorldContext = WorldContextObject))
    static UTextDisplayScreen *AddTextDisplayScreenToOverlay(const UObject *WorldContextObject);

  protected:
    void NativeConstruct() override;

  public:
    /**
     * Set the text to display to the player from this menu
     * @param TextToDisplay The text to display to the player
     */
    UFUNCTION(BlueprintCallable, Category = "Messages|Display")
    void SetText(FText TextToDisplay);

    /**
     * Display a set of choices to the player at the end of the message
     * @param TextToDisplay The text to display to the player
     * @param Choices The choices to choose from
     */
    UFUNCTION(BlueprintCallable, Category = "Messages|Display")
    void DisplayChoices(FText TextToDisplay, const TArray<FText> &Choices);

    /**
     * Clear all currently displaying text
     */
    UFUNCTION(BlueprintCallable, Category = "Messages|Display")
    void ClearDisplayText();

    /**
     * Assignable delegate for advancing to the next message
     */
    UPROPERTY(BlueprintAssignable, Category = "Messages|Flow Control")
    FNextMessage NextMessage;

    /**
     * Assignable delegate for when a choice is made
     */
    UPROPERTY(BlueprintAssignable, Category = "Messages|Flow Control")
    FChoiceMade ProcessChoice;

  private:
    /**
     * Function used to advance to the next message
     */
    UFUNCTION()
    void AdvanceToNextMessage();

    /**
     * Function to display a set of choices to the player
     */
    UFUNCTION()
    void DisplayChoicePrompt();

    /**
     * Process the choice made by the player
     * @param Index The selected index
     * @param Choice The choice selected
     */
    UFUNCTION()
    void ProcessSelectedChoice(int32 Index, UCommand *Choice);

    /**
     * The command window that is displayed to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UMessageWindow> MessageWindow;

    /**
     * The command window that is displayed to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCommandWindow> CommandWindow;
};

DECLARE_INJECTABLE_DEPENDENCY(POKEMONUI_API, UTextDisplayScreen)