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
#include "Screens/TextDisplayScreen.h"

#include "Data/Command.h"
#include "Windows/CommandWindow.h"
#include "Windows/MessageWindow.h"

void UTextDisplayScreen::NativeConstruct() {
	Super::NativeConstruct();
	if (MessageWindow == nullptr)
		return;
	
	MessageWindow->OnDisplayChoices.AddDynamic(this, &UTextDisplayScreen::UTextDisplayScreen::DisplayChoicePrompt);
	MessageWindow->OnAdvanceText.AddDynamic(this, &UTextDisplayScreen::AdvanceToNextMessage);
	MessageWindow->SetKeyboardFocus();

	CommandWindow->OnCommandSelected.AddDynamic(this, &UTextDisplayScreen::ProcessSelectedChoice);
}

void UTextDisplayScreen::SetText(FText TextToDisplay) {
	check(MessageWindow != nullptr);
	MessageWindow->ClearDisplayText();
	MessageWindow->SetDisplayText(TextToDisplay);
	CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
	MessageWindow->SetKeyboardFocus();
}

void UTextDisplayScreen::DisplayChoices(FText TextToDisplay, const TArray<FText>& Choices) {
	check(MessageWindow != nullptr && CommandWindow != nullptr);
	MessageWindow->ClearDisplayText();
	MessageWindow->SetDisplayText(TextToDisplay, true);

	CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
	TArray<TObjectPtr<UCommand>> Commands;
	for (const auto &Choice : Choices) {
		Commands.Add(UCommand::CreateBasicCommand(Choice));
	}
	CommandWindow->SetCommands(MoveTemp(Commands));
	MessageWindow->SetKeyboardFocus();
}

void UTextDisplayScreen::ClearDisplayText() {
	check(MessageWindow != nullptr);
	MessageWindow->ClearDisplayText();
	CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
}

void UTextDisplayScreen::AdvanceToNextMessage() {
	NextMessage.Broadcast();
}

void UTextDisplayScreen::DisplayChoicePrompt() {
	CommandWindow->SetIndex(0);
	CommandWindow->SetVisibility(ESlateVisibility::Visible);
	CommandWindow->SetActive(true);
	CommandWindow->SetKeyboardFocus();
}

void UTextDisplayScreen::ProcessSelectedChoice(int32 Index, UCommand *Choice) {
	ProcessChoice.Broadcast(Index, Choice->GetID());
	CommandWindow->SetActive(false);
}
