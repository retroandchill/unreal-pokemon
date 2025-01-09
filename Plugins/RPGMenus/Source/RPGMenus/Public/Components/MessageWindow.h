// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CommonActivatableWidget.h"
#include "Containers/Deque.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Functional/Delegates.h"

#include "MessageWindow.generated.h"

class UDialogueBox;
class UInputAction;
class UDisplayText;
class UCommandWindow;
class UScrollBox;
class UWindow;
class USizeBox;

/**
 * Delegate to what happens when the text finishes displaying and we need some type of user input (such as a command
 * input)
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDisplayChoices);

/**
 * Delegate to handle advancing the text in the message
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAdvanceText);

/**
 * Window for display text to the player
 */
UCLASS()
class RPGMENUS_API UMessageWindow : public UCommonActivatableWidget {
    GENERATED_BODY()

  protected:
    void NativeConstruct() override;
    void NativeDestruct() override;

    void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;

  public:
    /**
     * Set the text to display to the player
     * @param Text The text to display to the player
     */
    UFUNCTION(BlueprintCallable, Category = "Messages|Display")
    void SetDisplayText(FText Text, bool bHasCommands = false);

    /**
     * Clear all currently displaying text
     */
    UFUNCTION(BlueprintCallable, Category = "Messages|Display")
    void ClearDisplayText();

    void SetPaused(bool bPausedIn);

    /**
     * Is this message window awaiting player input to advance?
     * @return If the window is currently awaiting input
     */
    UFUNCTION(BlueprintPure, Category = "Messages|Input")
    bool IsAwaitingInput() const;

    RETRO_MULTICAST_DELEGATE_MEMBER(FSimpleMulticastDelegate, AdvanceTextReady);

    /**
     * Get the callback for when the text advances.
     *
     * @return The callback for when the text advances.
     */
    FAdvanceText &GetOnAdvanceText();

    /**
     * Get the callback for when choices need to be displayed to the player.
     *
     * @return The callback for displaying choices.
     */
    FDisplayChoices &GetOnDisplayChoices();

  private:
    /**
     * The callback for when the text has finished displaying
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FAdvanceText OnAdvanceText;

    /**
     * Callback for when we need to display a choice to the player
     */
    UPROPERTY(BlueprintAssignable, Category = Events)
    FDisplayChoices OnDisplayChoices;

    /**
     * The widget that contains the text displayed to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDialogueBox> DialogueBox;

    /**
     * The scroll box used to keep the text visible on screen
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> ScrollBox;

    /**
     * The size box used to control the desired height
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<USizeBox> SizeBox;

    /**
     * The arrow to show when the text is paused
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWidget> PauseArrow;

    /**
     * How much extra padding does a line have?
     */
    UPROPERTY(EditAnywhere, Category = Display)
    float ExtraPadding;

    /**
     * The original scroll position
     */
    TOptional<float> OriginalScroll;

    /**
     * The current timer for scrolling the window
     */
    TOptional<float> ScrollTimer;

    /**
     * The desired number of lines to display to the player
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 1, ClampMin = 1))
    int32 LinesToShow = 2;

    /**
     * The desired number of lines to display to the player
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 0.001f, ClampMin = 0.001f))
    float ScrollSpeed = 0.1f;

    /**
     * Is the current game state paused
     */
    bool bPaused = false;

    /**
     * Should we wait for commands at the end of the text display?
     */
    bool bWaitForChoice = false;

    /**
     * The input that when pressed will create a binding to the action
     */
    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<UInputAction> AdvanceActionInput;

    /**
     * The handle used to hold the advance action
     */
    FUIActionBindingHandle AdvanceAction;
};
