// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonRichTextBlock.h"
#include "CommonUserWidget.h"
#include "Framework/Text/IRun.h"

#include "DialogueBox.generated.h"

class FRichTextLayoutMarshaller;
class FSlateTextLayout;
/**
 * A text block that exposes more information about text layout.
 */
UCLASS()
class UDialogueTextBlock : public UCommonRichTextBlock
{
    GENERATED_BODY()

  public:
    /**
     * Retrieves the text layout instance associated with this text block.
     * The text layout defines how text is rendered and wrapped.
     *
     * @return A shared pointer to the FSlateTextLayout instance associated with this text block.
     */
    FORCEINLINE TSharedPtr<FSlateTextLayout> GetTextLayout() const
    {
        return TextLayout;
    }

    /**
     * Retrieves the text marshaller associated with this text block.
     * The text marshaller is responsible for managing rich text parsing and styling.
     *
     * @return A shared pointer to the FRichTextLayoutMarshaller instance associated with this text block.
     */
    FORCEINLINE TSharedPtr<FRichTextLayoutMarshaller> GetTextMarshaller() const
    {
        return TextMarshaller;
    }

  protected:
    TSharedRef<SWidget> RebuildWidget() override;

  private:
    TSharedPtr<FSlateTextLayout> TextLayout;
    TSharedPtr<FRichTextLayoutMarshaller> TextMarshaller;
};

/**
 * Represents a segment of dialogue text, including its content and associated run metadata.
 */
struct FDialogueTextSegment
{
    /**
     * Represents a string of text, typically used to store dialogue or text content.
     */
    FString Text;

    /**
     * Represents metadata and additional contextual information for a text run,
     * such as its name and associated attributes. Commonly utilized to distinguish
     * or categorize parts of a text layout or rich text format.
     */
    FRunInfo RunInfo;
};

/**
 * A widget representing a dialogue box capable of displaying text with a typewriter effect.
 *
 * This class provides functionality to display dialogue text with timed letter-by-letter appearance
 * and configurable timing options. Additional features include handling events when lines are
 * completed, skipping to the end of lines, and managing typed text segments.
 */
UCLASS()
class INTERACTIVEUI_API UDialogueBox : public UCommonUserWidget
{
    GENERATED_BODY()

  public:
    /**
     * Retrieves the text block associated with the current dialogue line.
     *
     * @return A pointer to the UDialogueTextBlock containing the current line's text.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    UDialogueTextBlock *GetLineText() const
    {
        return LineText.Get();
    }

    /**
     * Retrieves the time interval between individual letter displays for the "typewriter" effect.
     *
     * @return The duration in seconds for displaying each letter.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    float GetLetterPlayTime() const
    {
        return LetterPlayTime;
    }

    /**
     * Sets the time interval between individual letter displays for the "typewriter" effect.
     *
     * @param InLetterPlayTime The duration in seconds for displaying each letter.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetLetterPlayTime(const float InLetterPlayTime)
    {
        LetterPlayTime = InLetterPlayTime;
    }

    /**
     * Retrieves the amount of time to wait after completing the dialogue line
     * before marking it as finished. This waiting period is intended to prevent
     * accidental progression on short lines.
     *
     * @return The duration in seconds to hold after the line finishes.
     */
    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly)
    float GetEndHoldTime() const
    {
        return EndHoldTime;
    }

    /**
     * Sets the amount of time to wait after completing the dialogue line
     * before marking it as finished. This waiting period is intended to prevent
     * accidental progression on short lines.
     *
     * @param InEndHoldTime The duration in seconds to hold after the line finishes.
     */
    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly)
    void SetEndHoldTime(const float InEndHoldTime)
    {
        EndHoldTime = InEndHoldTime;
    }

    /**
     * Plays a dialogue line with a "typewriter" effect, displaying the text letter-by-letter.
     *
     * @param InLine The dialogue line text to be displayed.
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue Box")
    void PlayLine(const FText &InLine);

    /**
     * Retrieves the text of the current dialogue line.
     *
     * @return A reference to the FText representing the current dialogue line.
     */
    UFUNCTION(BlueprintPure, Category = "Dialogue Box")
    const FText &GetCurrentLine() const
    {
        return CurrentLine;
    }

    /**
     * Checks whether the current dialogue line has finished playing completely.
     *
     * @return True if the dialogue line has finished playing, otherwise false.
     */
    UFUNCTION(BlueprintPure, Category = "Dialogue Box", meta = (ScriptName = "GetHasFinishedPlayingLine"))
    bool HasFinishedPlayingLine() const
    {
        return bHasFinishedPlaying;
    }

    /**
     * Skips the current dialogue line to its end, bypassing the "typewriter" effect.
     *
     * Clears the timer responsible for sequentially displaying each letter,
     * marks the line as fully displayed by setting the current letter index
     * to the last letter, updates the displayed text accordingly, and notifies
     * relevant systems that the line has finished playing.
     *
     * Broadcasts the OnLineFinishedPlayingDelegate to signal any listeners that
     * the dialogue line playback is complete.
     */
    UFUNCTION(BlueprintCallable, Category = "Dialogue Box")
    void SkipToLineEnd();

    /**
     * Binds a delegate to the OnLineFinishedPlaying event.
     *
     * This function allows external systems to register a callback that will be
     * executed when a dialogue line finishes playing completely. The delegate
     * is added to the OnLineFinishedPlayingDelegate multicast delegate to ensure
     * it is called during the appropriate event broadcast.
     *
     * @param Delegate The delegate to be bound to the OnLineFinishedPlaying event.
     * @return A delegate handle that represents the bound delegate, allowing
     *         management or unbinding of the delegate.
     */
    FDelegateHandle BindToOnLineFinishedPlaying(FSimpleDelegate &&Delegate)
    {
        return OnLineFinishedPlayingDelegate.Add(MoveTemp(Delegate));
    }

    /**
     * Unbinds a delegate from the OnLineFinishedPlaying event.
     *
     * @param Handle The delegate handle to be removed.
     * @return Returns true if the delegate was successfully unbound; otherwise, false.
     */
    bool UnbindFromOnLineFinishedPlaying(const FDelegateHandle Handle)
    {
        return OnLineFinishedPlayingDelegate.Remove(Handle);
    }

  protected:
    /**
     * Event triggered to handle the playback of a letter in the dialogue box.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue Box")
    void OnPlayLetter();

    /**
     * Blueprint implementable event triggered when a dialogue line finishes playing.
     */
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue Box")
    void OnLineFinishedPlaying();

  private:
    void PlayNextLetter();

    void CalculateWrappedString();
    FString CalculateSegments();

    UPROPERTY(BlueprintGetter = GetLineText, meta = (BindWidget))
    TObjectPtr<UDialogueTextBlock> LineText;

    // The amount of time between printing individual letters (for the "typewriter" effect).
    UPROPERTY(EditAnywhere, BlueprintGetter = GetLetterPlayTime, BlueprintSetter = SetLetterPlayTime,
              Category = "Dialogue Box")
    float LetterPlayTime = 0.025f;

    // The amount of time to wait after finishing the line before actually marking it completed.
    // This helps prevent accidentally progressing dialogue on short lines.
    UPROPERTY(EditAnywhere, BlueprintGetter = GetEndHoldTime, BlueprintSetter = SetEndHoldTime,
              Category = "Dialogue Box")
    float EndHoldTime = 0.15f;

    UPROPERTY()
    FText CurrentLine;

    TArray<FDialogueTextSegment> Segments;

    // The section of the text that's already been printed out and won't ever change.
    // This lets us cache some of the work we've already done. We can't cache absolutely
    // everything as the last few characters of a string may change if they're related to
    // a named run that hasn't been completed yet.
    FString CachedSegmentText;
    int32 CachedLetterIndex = 0;

    int32 CurrentSegmentIndex = 0;
    int32 CurrentLetterIndex = 0;
    int32 MaxLetterIndex = 0;

    uint32 bHasFinishedPlaying : 1 = true;

    FTimerHandle LetterTimer;

    FSimpleMulticastDelegate OnLineFinishedPlayingDelegate;
};
