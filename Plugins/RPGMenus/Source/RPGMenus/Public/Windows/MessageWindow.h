// "Unreal Pokémon" created by Retro & Chill.
#pragma once

#include "Blueprint/UserWidget.h"
#include "Containers/Deque.h"
#include "CoreMinimal.h"
#include "MessageWindow.generated.h"

class UCommandWindow;
class UScrollBox;
class UWindow;
class USizeBox;
class UDisplayText;
class USelectionInputs;

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
class RPGMENUS_API UMessageWindow : public UUserWidget {
    GENERATED_BODY()

  public:
    TSharedRef<SWidget> RebuildWidget() override;
    void SynchronizeProperties() override;
    void PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) override;

  protected:
    void NativeTick(const FGeometry &MyGeometry, float InDeltaTime) override;
    FReply NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) override;
    void NativeOnFocusLost(const FFocusEvent &InFocusEvent) override;

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
    UFUNCTION(BlueprintPure, CAtegory = "Messages|Input")
    bool IsAwaitingInput() const;

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
     * Automatically resize the window to match the desired number of lines
     */
    void ResizeWindow();

    /**
     * Queue up the new text if the geometry information is available
     */
    void QueueUpNewText();

    /**
     * Queue up a new line of text to display to the player
     * @param Line The current line of text to queue
     * @param TotalTextAreaWidth The total width of the rendering area
     */
    void QueueLine(const FString &Line, double TotalTextAreaWidth);

    /**
     * Queue the given text to the display queue
     * @param Text The text to add to the queue
     */
    void QueueText(FStringView Text);

    /**
     * Split up the line by word and add each word individually wrapping when necessary
     * @param Line The current line of text to queue
     * @param TotalTextAreaWidth The total width of the rendering area
     */
    void QueueIndividualWords(const FString &Line, double TotalTextAreaWidth);

    /**
     * Queue a new line to be added
     */
    void AddNewLine();

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
     * The actual area where the window is drawn
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UWindow> Window;

    /**
     * The widget that contains the text displayed to the player
     */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UDisplayText> DisplayTextWidget;

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
     * Do the selection options wrap when input would exceed the end
     */
    UPROPERTY(EditAnywhere, Category = Input)
    TObjectPtr<USelectionInputs> InputMappings;

    /**
     * The full text to queue up for display
     */
    TOptional<FText> FullText;

    /**
     * The current word to be displayed to the player
     */
    TQueue<TCHAR> WordToDisplay;

    /**
     * The speed at which the text scrolls by (0 = Instant)
     */
    UPROPERTY(EditAnywhere, Category = Display, meta = (UIMin = 0, UIMax = 0))
    float TextSpeed = 0.025f;

    /**
     * The current timer for the text letter display
     */
    float TextTimer = 0.0f;

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
};
