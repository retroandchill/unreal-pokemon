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

#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Primatives/DisplayText.h"

TSharedRef<SWidget> UMessageWindow::RebuildWidget() {
	auto Ret = Super::RebuildWidget();

	if (SizeBox != nullptr && DisplayTextWidget != nullptr) {
		double TextHeight = DisplayTextWidget->GetTextSize("Sample").Y;
		SizeBox->SetHeightOverride(TextHeight * LinesToShow);
	}
	
	return Ret;
}

void UMessageWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	QueueUpNewText();

	if (WordToDisplay.IsEmpty())
		return;

	TextTimer += InDeltaTime;
	if (TextTimer < TextSpeed)
		return;
	
	TCHAR NextChar;
	WordToDisplay.Dequeue(NextChar);
	auto NewText = DisplayTextWidget->GetText().ToString();
	NewText.AppendChar(NextChar);
	ScrollBox->ScrollToEnd();
	DisplayTextWidget->SetText(FText::FromString(NewText));
	TextTimer = 0;
}

void UMessageWindow::SetDisplayText(FText Text) {
	check(DisplayTextWidget != nullptr);
	
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

void UMessageWindow::QueueUpNewText() {
	if (!FullText.IsSet())
		return;
	
	double TotalTextAreaWidth = DisplayTextWidget->GetTotalTextAreaSize().X;
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
		} else {
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