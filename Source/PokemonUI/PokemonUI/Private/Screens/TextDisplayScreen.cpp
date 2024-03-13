// "Unreal Pokémon" created by Retro & Chill.
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
	check(MessageWindow != nullptr)
	MessageWindow->ClearDisplayText();
	MessageWindow->SetDisplayText(TextToDisplay);
	CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
	MessageWindow->SetKeyboardFocus();
}

void UTextDisplayScreen::DisplayChoices(FText TextToDisplay, const TArray<FText>& Choices) {
	check(MessageWindow != nullptr && CommandWindow != nullptr)
	MessageWindow->ClearDisplayText();
	MessageWindow->SetDisplayText(TextToDisplay, true);

	CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
	TArray<TObjectPtr<UCommand>> Commands;
	for (const auto& Choice : Choices) {
		Commands.Add(UCommand::CreateBasicCommand(Choice));
	}
	CommandWindow->SetCommands(MoveTemp(Commands));
	MessageWindow->SetKeyboardFocus();
}

void UTextDisplayScreen::ClearDisplayText() {
	check(MessageWindow != nullptr)
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

void UTextDisplayScreen::ProcessSelectedChoice(int32 Index, UCommand* Choice) {
	ProcessChoice.Broadcast(Index, Choice->GetID());
	CommandWindow->SetActive(false);
}
