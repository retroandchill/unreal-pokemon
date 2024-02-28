//====================================================================================================================
// ** Unreal Pokémon created by Retro & Chill
//--------------------------------------------------------------------------------------------------------------------
// This project is intended as a means of learning more about how a game like Pokémon works by creating a framework
// from the ground up, and for non-commercial applications. While this code is original, Pokémon is the intellectual
// property of Game Freak and Nintendo, as such it is highly discouraged to use this kit to make a commercial product.
//--------------------------------------------------------------------------------------------------------------------
// THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//====================================================================================================================
#include "Windows/MessageWindow.h"

#include "MathUtilities.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Data/SelectionInputs.h"
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

void UMessageWindow::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ResizeWindow();
}

void UMessageWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
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
		ScrollBox->SetScrollOffset(UMathUtilities::LinearInterpolationF(OriginalScroll.GetValue(), BottomScroll, ScrollSpeed, ScrollTimer.GetValue()));
		
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

FReply UMessageWindow::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) {
	if (InputMappings == nullptr || !InputMappings->IsConfirmInput(InKeyEvent.GetKey()) || !bPaused)
		return FReply::Unhandled();

	if (WordToDisplay.IsEmpty() && !bWaitForChoice) {
		OnAdvanceText.Broadcast();
	} else if (bPaused) {
		SetPaused(false);
	}
	return FReply::Handled();
}

void UMessageWindow::NativeOnFocusLost(const FFocusEvent& InFocusEvent) {
	Super::NativeOnFocusLost(InFocusEvent);
	
	if (InFocusEvent.GetCause() == EFocusCause::Mouse) {
		SetKeyboardFocus();
	}
}

void UMessageWindow::SetDisplayText(FText Text, bool bHasCommands) {
	check(DisplayTextWidget != nullptr);

	bWaitForChoice = bHasCommands;
	if (FMath::IsNearlyZero(TextSpeed)) {
		DisplayTextWidget->SetText(Text);
	} else {
		FullText = MoveTemp(Text);
	}
}

void UMessageWindow::ClearDisplayText() {
	check(DisplayTextWidget != nullptr);
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

void UMessageWindow::ResizeWindow() {
	if (SizeBox != nullptr && DisplayTextWidget != nullptr) {
		double TextHeight = DisplayTextWidget->GetTextSize("Sample").Y;
		auto DisplayTextPadding = DisplayTextWidget->GetDisplayTextPadding();
		SizeBox->SetHeightOverride(TextHeight * LinesToShow + DisplayTextPadding.Top + DisplayTextPadding.Bottom);
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

void UMessageWindow::QueueLine(const FString& Line, double TotalTextAreaWidth) {
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

void UMessageWindow::QueueIndividualWords(const FString& Line, double TotalTextAreaWidth) {
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

void UMessageWindow::AddNewLine() {
	QueueText(LINE_TERMINATOR);
}