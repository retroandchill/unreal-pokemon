// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Framework/Text/IRun.h"

#include "DialogueBox.generated.h"

class UDialogueTextBlock;

DECLARE_MULTICAST_DELEGATE(FOnLineFinishedPlaying);

struct FDialogueTextSegment
{
    FString Text;
    FRunInfo RunInfo;
};

/**
 *
 */
UCLASS()
class RPGMENUS_API UDialogueBox : public UCommonUserWidget
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Dialogue Box", meta = (AutoCreateRefTerm = InLine))
    void PlayLine(const FText &InLine);

    UFUNCTION(BlueprintCallable, Category = "Dialogue Box")
    const FText &GetCurrentLine() const
    {
        return CurrentLine;
    }

    UFUNCTION(BlueprintCallable, Category = "Dialogue Box")
    bool HasFinishedPlayingLine() const
    {
        return bHasFinishedPlaying;
    }

    UFUNCTION(BlueprintCallable, Category = "Dialogue Box")
    void SkipToLineEnd();

    FDelegateHandle BindToOnLineFinishedPlaying(FOnLineFinishedPlaying::FDelegate &&Callback);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = Widgets)
    UDialogueTextBlock *GetLineText() const;

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "DialogueBox")
    float GetLetterPlayTime() const;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "DialogueBox")
    void SetLetterPlayTime(float NewPlayTime);

    UFUNCTION(BlueprintPure, BlueprintInternalUseOnly, Category = "DialogueBox")
    float GetEndHoldTime() const;

    UFUNCTION(BlueprintCallable, BlueprintInternalUseOnly, Category = "DialogueBox")
    void SetEndHoldTime(float NewHoldTime);

  protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue Box")
    void OnPlayLetter();

    UFUNCTION(BlueprintImplementableEvent, Category = "Dialogue Box")
    void OnLineFinishedPlaying();

  private:
    void PlayNextLetter();

    void CalculateWrappedString();
    FString CalculateSegments();
    static void ProcessSegmentTags(FString &Result, int32 &Idx, const FDialogueTextSegment &Segment);

    UPROPERTY(BlueprintGetter = GetLineText, Category = Widgets, meta = (BindWidget))
    TObjectPtr<UDialogueTextBlock> LineText;

    /**
     * The amount of time between printing individual letters (for the "typewriter" effect).
     */
    UPROPERTY(EditAnywhere, BlueprintGetter = GetLetterPlayTime, BlueprintSetter = SetLetterPlayTime,
              Category = "Dialogue Box")
    float LetterPlayTime = 0.025f;

    /**
     * The amount of time to wait after finishing the line before actually marking it completed. This helps prevent
     * accidentally progressing dialogue on short lines.
     */
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

    bool bHasFinishedPlaying = true;

    FTimerHandle LetterTimer;

    FOnLineFinishedPlaying LineFinishedPlayingDelegate;
};
