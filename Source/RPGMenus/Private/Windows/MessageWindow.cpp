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

#include "Primatives/DisplayText.h"

void UMessageWindow::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (TextToDisplay.IsEmpty())
		return;

	TextTimer += InDeltaTime;
	if (TextTimer < TextSpeed)
		return;
	
	if (!WordToDisplay.IsEmpty()) {
		TCHAR NextChar;
		WordToDisplay.Dequeue(NextChar);
		auto NewText = DisplayTextWidget->GetText().ToString();
		NewText.AppendChar(NextChar);
	} else {
		auto &CurrentLine = *TextToDisplay.Peek();
		double TotalTextAreaWidth = DisplayTextWidget->GetTotalTextAreaSize().X;

		auto &CurrentWord = *CurrentLine.Peek();
		auto NewText = FString(" ") + CurrentWord;
		double CurrentTextAreaWidth = DisplayTextWidget->GetTextSize().X;
		double NewTextWidth = DisplayTextWidget->GetTextSize(NewText).X;
		if (double FullTextWidth = CurrentTextAreaWidth + NewTextWidth; FullTextWidth > TotalTextAreaWidth) {
			for (FString LineTerminator = LINE_TERMINATOR; auto Character : LineTerminator) {
				WordToDisplay.Enqueue(Character);
			}
		} else {
			WordToDisplay.Enqueue(TEXT(' '));
		}

		for (auto Character : CurrentWord) {
			WordToDisplay.Enqueue(Character);
		}
	}
	TextTimer = 0;
}

void UMessageWindow::SetDisplayText(FText Text) {
	if (FMath::IsNearlyZero(TextSpeed)) {
		DisplayTextWidget->SetText(Text);
		return;
	} else {
		auto &AsString = Text.ToString();
		TArray<FString> Lines;
		FString::ParseIntoArray(Lines, LINE_TERMINATOR);
		for (const auto &Line : Lines) {
			TextToDisplay.Enqueue(TQueue<FString>());
			auto &CurrentLine = *TextToDisplay.Peek();
			TArray<FString> Words;
			FString::ParseIntoArray(Words, TEXT(" "));
			for (auto &Word : Words) {
				CurrentLine.Enqueue(MoveTemp(Word));
			}
		}
	}
}
