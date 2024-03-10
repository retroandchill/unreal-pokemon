// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/PokemonSelectionPane.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Managers/PokemonSubsystem.h"
#include "Primatives/PokemonPanel.h"
#include "Primatives/PartySelectCancelPanel.h"

void UPokemonSelectionPane::NativeConstruct() {
	Super::NativeConstruct();

	check(ContentsArea != nullptr)
	auto& PokemonSubsystem = UPokemonSubsystem::GetInstance();
	auto& PlayerParty = PokemonSubsystem.GetPlayer().GetParty();

	ActivePanels.Empty();
	for (int32 i = 0; i < PokemonSubsystem.GetMaxPartySize(); i++) {
		if (i < PlayerParty.Num()) {
			auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass);
			NewWidget->SetOwner(this);
			NewWidget->SetPokemon(PlayerParty[i], i);
			auto PanelSlot = ContentsArea->AddChildToCanvas(NewWidget);
			PanelSlot->SetOffsets(GetPanelOffset(i));
			ActivePanels.Add(NewWidget);
		} else {
			auto NewWidget = WidgetTree->ConstructWidget<UWidget>(BlankPanelClass);
			auto PanelSlot = ContentsArea->AddChildToCanvas(NewWidget);
			PanelSlot->SetOffsets(GetPanelOffset(i));
		}
	}

	MultiSelectConfirmPanel->SetOwner(this);
	MultiSelectCancelPanel->SetOwner(this);
	CancelPanel->SetOwner(this);

	if (bMultiSelectMode) {
		ConfirmCancelSwitcher->SetActiveWidgetIndex(1);
		AddAdditionalPanelToOptions(MultiSelectConfirmPanel);
		AddAdditionalPanelToOptions(MultiSelectCancelPanel);
	} else {
		ConfirmCancelSwitcher->SetActiveWidgetIndex(0);
		AddAdditionalPanelToOptions(CancelPanel);
	}
}

int32 UPokemonSelectionPane::GetItemCount_Implementation() const {
	return ActivePanels.Num();
}

int32 UPokemonSelectionPane::GetColumnCount_Implementation() const {
	return Columns;
}

bool UPokemonSelectionPane::IsMultiSelectMode() const {
	return bMultiSelectMode;
}

void UPokemonSelectionPane::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
	for (ISelectablePanel* Panel : ActivePanels) {
		Panel->Refresh();
	}
}

void UPokemonSelectionPane::AddAdditionalPanelToOptions(TObjectPtr<UPartySelectCancelPanel>& Panel) {
	Panel->SetMenuIndex(ActivePanels.Num());
	ActivePanels.Add(Panel);
}
