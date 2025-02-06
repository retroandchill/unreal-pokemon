// "Unreal Pokémon" created by Retro & Chill.

#include "Components/DialogueBox.h"
#include "Components/DialogueTextBlock.h"
#include "Framework/Text/ILayoutBlock.h"
#include "Framework/Text/RichTextLayoutMarshaller.h"
#include "Framework/Text/SlateTextLayout.h"
#include "TimerManager.h"

void UDialogueBox::PlayLine(const FText &InLine) {
    check(GetWorld() != nullptr)

    FTimerManager &TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer(LetterTimer);

    CurrentLine = InLine;
    CurrentLetterIndex = 0;
    CachedLetterIndex = 0;
    CurrentSegmentIndex = 0;
    MaxLetterIndex = 0;
    Segments.Empty();
    CachedSegmentText.Empty();

    if (CurrentLine.IsEmpty()) {
        if (IsValid(LineText)) {
            LineText->SetText(FText::GetEmpty());
        }

        bHasFinishedPlaying = true;
        LineFinishedPlayingDelegate.Broadcast();
        OnLineFinishedPlaying();

        SetVisibility(ESlateVisibility::Hidden);
    } else {
        if (IsValid(LineText)) {
            LineText->SetText(FText::GetEmpty());
        }

        bHasFinishedPlaying = false;

        FTimerDelegate Delegate;
        Delegate.BindUObject(this, &ThisClass::PlayNextLetter);

        TimerManager.SetTimer(LetterTimer, Delegate, LetterPlayTime, true);

        SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    }
}

void UDialogueBox::SkipToLineEnd() {
    FTimerManager &TimerManager = GetWorld()->GetTimerManager();
    TimerManager.ClearTimer(LetterTimer);

    CurrentLetterIndex = MaxLetterIndex - 1;
    if (IsValid(LineText)) {
        LineText->SetText(FText::FromString(CalculateSegments()));
    }

    bHasFinishedPlaying = true;
    LineFinishedPlayingDelegate.Broadcast();
    OnLineFinishedPlaying();
}

FDelegateHandle UDialogueBox::BindToOnLineFinishedPlaying(FOnLineFinishedPlaying::FDelegate &&Callback) {
    return LineFinishedPlayingDelegate.Add(std::move(Callback));
}

UDialogueTextBlock *UDialogueBox::GetLineText() const {
    return LineText;
}

float UDialogueBox::GetLetterPlayTime() const {
    return LetterPlayTime;
}

void UDialogueBox::SetLetterPlayTime(float NewPlayTime) {
    LetterPlayTime = NewPlayTime;
}

float UDialogueBox::GetEndHoldTime() const {
    return EndHoldTime;
}

void UDialogueBox::SetEndHoldTime(float NewHoldTime) {
    EndHoldTime = NewHoldTime;
}

void UDialogueBox::PlayNextLetter() {
    if (Segments.IsEmpty()) {
        CalculateWrappedString();
    }

    FString WrappedString = CalculateSegments();

    // TODO: How do we keep indexing of text i18n-friendly?
    if (CurrentLetterIndex < MaxLetterIndex) {
        if (IsValid(LineText)) {
            LineText->SetText(FText::FromString(WrappedString));
        }

        OnPlayLetter();
        ++CurrentLetterIndex;
    } else {
        if (IsValid(LineText)) {
            LineText->SetText(FText::FromString(CalculateSegments()));
        }

        FTimerManager &TimerManager = GetWorld()->GetTimerManager();
        TimerManager.ClearTimer(LetterTimer);

        FTimerDelegate Delegate;
        Delegate.BindUObject(this, &ThisClass::SkipToLineEnd);

        TimerManager.SetTimer(LetterTimer, Delegate, EndHoldTime, false);
    }
}

void UDialogueBox::CalculateWrappedString() {
    if (!IsValid(LineText) || !LineText->GetTextLayout().IsValid()) {
        Segments.Add(FDialogueTextSegment{CurrentLine.ToString()});
        MaxLetterIndex = Segments[0].Text.Len();
        return;
    }

    TSharedPtr<FSlateTextLayout> Layout = LineText->GetTextLayout();
    TSharedPtr<FRichTextLayoutMarshaller> Marshaller = LineText->GetTextMarshaller();

    const FGeometry &TextBoxGeometry = LineText->GetCachedGeometry();
    const FVector2D TextBoxSize = TextBoxGeometry.GetLocalSize();

    Layout->SetWrappingWidth(static_cast<float>(TextBoxSize.X));
    Marshaller->SetText(CurrentLine.ToString(), *Layout.Get());
    Layout->UpdateIfNeeded();

    bool bHasWrittenText = false;
    for (const FTextLayout::FLineView &View : Layout->GetLineViews()) {
        for (TSharedRef<ILayoutBlock> Block : View.Blocks) {
            TSharedRef<IRun> Run = Block->GetRun();

            FDialogueTextSegment Segment;
            Run->AppendTextTo(Segment.Text, Block->GetTextRange());

            // HACK: For some reason image decorators (and possibly other decorators that don't
            // have actual text inside them) result in the run containing a zero width space instead of
            // nothing. This messes up our checks for whether the text is empty or not, which doesn't
            // have an effect on image decorators but might cause issues for other custom ones.
            if (Segment.Text.Len() == 1 && Segment.Text[0] == 0x200B) {
                Segment.Text.Empty();
            }

            Segment.RunInfo = Run->GetRunInfo();
            Segments.Add(Segment);

            // A segment with a named run should still take up time for the typewriter effect.
            MaxLetterIndex += FMath::Max(Segment.Text.Len(), Segment.RunInfo.Name.IsEmpty() ? 0 : 1);

            if (!Segment.Text.IsEmpty() || !Segment.RunInfo.Name.IsEmpty()) {
                bHasWrittenText = true;
            }
        }

        if (bHasWrittenText) {
            Segments.Add(FDialogueTextSegment{TEXT("\n")});
            ++MaxLetterIndex;
        }
    }

    Layout->SetWrappingWidth(0);
    LineText->SetText(LineText->GetText());
}

FString UDialogueBox::CalculateSegments() {
    FString Result = CachedSegmentText;

    int32 Idx = CachedLetterIndex;
    while (Idx <= CurrentLetterIndex && CurrentSegmentIndex < Segments.Num()) {
        const FDialogueTextSegment &Segment = Segments[CurrentSegmentIndex];
        ProcessSegmentTags(Result, Idx, Segment);

        bool bIsSegmentComplete = true;
        if (!Segment.Text.IsEmpty()) {
            int32 LettersLeft = CurrentLetterIndex - Idx + 1;
            bIsSegmentComplete = LettersLeft >= Segment.Text.Len();
            LettersLeft = FMath::Min(LettersLeft, Segment.Text.Len());
            Idx += LettersLeft;

            Result += Segment.Text.Mid(0, LettersLeft);

            if (!Segment.RunInfo.Name.IsEmpty()) {
                Result += TEXT("</>");
            }
        }

        if (!bIsSegmentComplete) {
            break;
        }

        CachedLetterIndex = Idx;
        CachedSegmentText = Result;
        ++CurrentSegmentIndex;
    }

    return Result;
}

void UDialogueBox::ProcessSegmentTags(FString &Result, int32 &Idx, const FDialogueTextSegment &Segment) {
    if (!Segment.RunInfo.Name.IsEmpty()) {
        Result += FString::Printf(TEXT("<%s"), *Segment.RunInfo.Name);

        if (!Segment.RunInfo.MetaData.IsEmpty()) {
            for (const TTuple<FString, FString> &MetaData : Segment.RunInfo.MetaData) {
                Result += FString::Printf(TEXT(" %s=\"%s\""), *MetaData.Key, *MetaData.Value);
            }
        }

        if (Segment.Text.IsEmpty()) {
            Result += TEXT("/>");
            ++Idx; // This still takes up an index for the typewriter effect.
        } else {
            Result += TEXT(">");
        }
    }
}