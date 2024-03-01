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

FText UCommand::GetOriginalText() const {
	return Text;
}
