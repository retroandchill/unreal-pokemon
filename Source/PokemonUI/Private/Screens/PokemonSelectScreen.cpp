// "Unreal Pokémon" created by Retro & Chill.
#include "Screens/PokemonSelectScreen.h"

#include "Data/Command.h"
#include "Managers/PokemonSubsystem.h"
#include "Windows/CommandWindow.h"
#include "Windows/HelpWindow.h"
#include "Windows/PokemonSelectionPane.h"

void UPokemonSelectScreen::NativeConstruct() {
	Super::NativeConstruct();
	check(SelectionPane != nullptr);
	SelectionPane->SetIndex(0);
	SelectionPane->SetActive(true);
	SelectionPane->OnConfirm.AddDynamic(this, &UPokemonSelectScreen::OnPokemonSelected);
	SelectionPane->OnCancel.AddDynamic(this, &UPokemonSelectScreen::CloseScreen);
	ToggleCommandWindowVisibility(false);
	
}

void UPokemonSelectScreen::OnPokemonSelected(int32 Index) {
	auto &Party = UPokemonSubsystem::GetInstance().GetPlayer().GetParty();
	if (Index < Party.Num()) {
		TArray<TObjectPtr<UCommand>> Commands;
		Commands.Add(UCommand::CreateBasicCommand(NSLOCTEXT("UPokemonSelectScreen", "Commands_Summary", "Summary")));
		Commands.Add(UCommand::CreateBasicCommand(NSLOCTEXT("UPokemonSelectScreen", "Commands_Switch", "Switch")));
		Commands.Add(UCommand::CreateBasicCommand(NSLOCTEXT("UPokemonSelectScreen", "Commands_Item", "Item")));
		Commands.Add(UCommand::CreateBasicCommand(NSLOCTEXT("UPokemonSelectScreen", "Commands_Cancel", "Cancel")));
		CommandWindow->SetCommands(MoveTemp(Commands));
		
		SelectionPane->SetActive(false);
		CommandWindow->SetIndex(0);
		CommandWindow->SetActive(true);
		
		SelectionPane->ToggleCommandVisibility(false);
		ToggleCommandWindowVisibility(true);

	} else {
		// TODO: Handle the additional options
	}
}

void UPokemonSelectScreen::ToggleCommandWindowVisibility(bool bIsVisible) {
	if (bIsVisible) {
		CommandWindow->SetVisibility(ESlateVisibility::Visible);
		CommandHelpWindow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	} else {
		CommandWindow->SetVisibility(ESlateVisibility::Collapsed);
		CommandHelpWindow->SetVisibility(ESlateVisibility::Collapsed);
	}
}
