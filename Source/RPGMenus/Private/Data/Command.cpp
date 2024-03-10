// "Unreal Pokémon" created by Retro & Chill.
#include "Data/Command.h"

FName UCommand::GetID() const {
	return ID;
}

FText UCommand::GetText_Implementation() const {
	return Text;
}

bool UCommand::IsEnabled_Implementation() const {
	return true;
}

UCommand* UCommand::CreateBasicCommand(const FText& Text) {
	auto Command = NewObject<UCommand>();

	auto NormalizedText = Text.ToString();
	NormalizedText.RemoveSpacesInline();

	Command->ID = FName(*NormalizedText);
	Command->Text = Text;

	return Command;
}

UCommand* UCommand::CreateBasicCommand(FName ID, const FText& Text, UObject* Handler) {
	auto Command = NewObject<UCommand>();

	Command->ID = ID;
	Command->Text = Text;
	Command->Handler = Handler;

	return Command;
}

void UCommand::ExecuteCommand_Implementation(ARPGPlayerController* Controller) {
	// No implementation in this class, this is simply used by the child classes to handle the effects
}

FText UCommand::GetOriginalText() const {
	return Text;
}
