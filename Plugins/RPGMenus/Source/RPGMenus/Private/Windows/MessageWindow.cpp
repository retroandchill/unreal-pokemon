// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/MessageWindow.h"

#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Data/SelectionInputs.h"
#include "MathUtilities.h"
#include "Primatives/DisplayText.h"
#include "Utilities/WidgetUtilities.h"

TSharedRef<SWidget> UMessageWindow::RebuildWidget() {
    auto Ret = Super::RebuildWidget();
    ResizeWindow();
    return Ret;
}

void UMessageWindow::SynchronizeProperties() {
    Super::SynchronizeProperties();
    ResizeWindow();
}

void UMessageWindow::PostEditChangeProperty(FPropertyChangedEvent &PropertyChangedEvent) {
    Super::PostEditChangeProperty(PropertyChangedEvent);
    ResizeWindow();
}

void UMessageWindow::NativeTick(const FGeometry &MyGeometry, float InDeltaTime) {
    Super::NativeTick(MyGeometry, InDeltaTime);

    QueueUpNewText();

    if (bPaused) {
        if (DisplayTextWidget->GetText().IsEmpty()) {
            SetPaused(false);
        } else {
            return;
        }
    }

    if (float BottomScroll = ScrollBox->GetScrollOffsetOfEnd(); ScrollTimer.IsSet() && OriginalScroll.IsSet()) {
        ScrollTimer.GetValue() += InDeltaTime;
        ScrollBox->SetScrollOffset(UMathUtilities::LinearInterpolation(OriginalScroll.GetValue(), BottomScroll,
                                                                       ScrollSpeed, ScrollTimer.GetValue()));

        if (FMath::IsNearlyEqual(ScrollBox->GetScrollOffset(), BottomScroll)) {
            ScrollTimer.Reset();
            OriginalScroll.Reset();
        }
    } else if (float CurrentScroll = ScrollBox->GetScrollOffset(); !FMath::IsNearlyEqual(CurrentScroll, BottomScroll)) {
        ScrollTimer.Emplace(0.f);
        OriginalScroll.Emplace(CurrentScroll);

        if (!DisplayTextWidget->GetText().IsEmpty()) {
            SetPaused(true);
        }
    }

    if (WordToDisplay.IsEmpty())
        return;

    TextTimer += InDeltaTime;
    if (TextTimer < TextSpeed)
        return;

    TCHAR NextChar;
    WordToDisplay.Dequeue(NextChar);
    auto NewText = DisplayTextWidget->GetText().ToString();
    NewText.AppendChar(NextChar);
    DisplayTextWidget->SetText(FText::FromString(NewText));
    TextTimer = 0;

    if (WordToDisplay.IsEmpty()) {
        SetPaused(true);

        if (bWaitForChoice) {
            OnDisplayChoices.Broadcast();
        }
    }
}

FReply UMessageWindow::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent) {
    if (InputMappings == nullptr || !InputMappings->IsConfirmInput(InKeyEvent.GetKey()) || !bPaused)
        return FReply::Unhandled();

    if (WordToDisplay.IsEmpty() && !bWaitForChoice) {
        OnAdvanceText.Broadcast();
    } else if (bPaused) {
        SetPaused(false);
    }
    return FReply::Handled();
}

void UMessageWindow::NativeOnFocusLost(const FFocusEvent &InFocusEvent) {
    Super::NativeOnFocusLost(InFocusEvent);

    if (InFocusEvent.GetCause() == EFocusCause::Mouse) {
        SetKeyboardFocus();
    }
}

void UMessageWindow::SetDisplayText(FText Text, bool bHasCommands) {
    check(DisplayTextWidget != nullptr)

        bWaitForChoice = bHasCommands;
    if (FMath::IsNearlyZero(TextSpeed)) {
        DisplayTextWidget->SetText(Text);
    } else {
        FullText = MoveTemp(Text);
    }
}

void UMessageWindow::ClearDisplayText() {
    check(DisplayTextWidget != nullptr)
DisplayTextWidget->SetText(FText::FromString(TEXT("")));
    WordToDisplay.Empty();
    FullText.Reset();
}

void UMessageWindow::SetPaused(bool bPausedIn) {
    bPaused = bPausedIn;

    if (PauseArrow == nullptr)
        return;

    if (bPaused && !WordToDisplay.IsEmpty()) {
        PauseArrow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
    } else {
        PauseArrow->SetVisibility(ESlateVisibility::Hidden);
    }
}

bool UMessageWindow::IsAwaitingInput() const { return bPaused; }

void UMessageWindow::ResizeWindow() {
    if (SizeBox != nullptr && DisplayTextWidget != nullptr) {
        auto TextHeight = static_cast<float>(DisplayTextWidget->GetTextSize("Sample").Y);
        auto DisplayTextPadding = DisplayTextWidget->GetDisplayTextPadding();
        SizeBox->SetHeightOverride(TextHeight * static_cast<float>(LinesToShow) + DisplayTextPadding.Top + DisplayTextPadding.Bottom);
    }
}

void UMessageWindow::QueueUpNewText() {
    if (!FullText.IsSet())
        return;

    double TotalTextAreaWidth = DisplayTextWidget->GetTotalTextAreaSize().X * UWidgetUtilities::GetWidgetDPIScale();
    if (FMath::IsNearlyZero(TotalTextAreaWidth))
        return;

    auto &AsString = FullText.GetValue().ToString();
    TArray<FString> Lines;
    AsString.ParseIntoArray(Lines, LINE_TERMINATOR);
    for (const auto &Line : Lines) {
        QueueLine(Line, TotalTextAreaWidth);
    }

    FullText.Reset();
}

void UMessageWindow::QueueLine(const FString &Line, double TotalTextAreaWidth) {
    if (double LineWidth = DisplayTextWidget->GetTextSize(Line).X; TotalTextAreaWidth >= LineWidth) {
        QueueText(Line);
    } else {
        QueueIndividualWords(Line, TotalTextAreaWidth);
    }
}

void UMessageWindow::QueueText(FStringView Text) {
    for (auto Char : Text) {
        WordToDisplay.Enqueue(Char);
    }
}

void UMessageWindow::QueueIndividualWords(const FString &Line, double TotalTextAreaWidth) {
    TArray<FString> Words;
    Line.ParseIntoArray(Words, TEXT(" "));
    FString CurrentLine = "";
    for (auto &Word : Words) {
        FString NewText = CurrentLine.IsEmpty() ? Word : FString(" ") + Word;
        double CurrentTextWidth = DisplayTextWidget->GetTextSize(CurrentLine).X;
        double NewTextWidth = DisplayTextWidget->GetTextSize(NewText).X;

        if (double FullTextWidth = CurrentTextWidth + NewTextWidth; FullTextWidth > TotalTextAreaWidth) {
            AddNewLine();
            CurrentLine = "";
        } else if (!WordToDisplay.IsEmpty()) {
            QueueText(TEXT(" "));
            CurrentLine += " ";
        }

        QueueText(Word);
        CurrentLine += Word;
    }
}

void UMessageWindow::AddNewLine() { QueueText(LINE_TERMINATOR); }
