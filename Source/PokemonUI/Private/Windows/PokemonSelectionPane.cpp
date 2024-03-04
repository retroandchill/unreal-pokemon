// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/PokemonSelectionPane.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Managers/PokemonSubsystem.h"
#include "Primatives/PokemonPanel.h"

void UPokemonSelectionPane::NativeConstruct() {
	Super::NativeConstruct();

	check(ContentsArea != nullptr);
	auto &PokemonSubsystem = UPokemonSubsystem::GetInstance();
	auto &PlayerParty = PokemonSubsystem.GetPlayer().GetParty();
	
	for (int32 i = 0; i < PlayerParty.Num(); i++) {
		auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass);
		NewWidget->SetOwner(this);
		NewWidget->SetPokemon(PlayerParty[i], i);
		auto PanelSlot = ContentsArea->AddChildToCanvas(NewWidget);
		PanelSlot->SetOffsets(GetPanelOffset(i));
	}
}

int32 UPokemonSelectionPane::GetItemCount_Implementation() const {
	auto &PokemonSubsystem = UPokemonSubsystem::GetInstance();
	return PokemonSubsystem.GetPlayer().GetParty().Num();
}

int32 UPokemonSelectionPane::GetColumnCount_Implementation() const {
	return Columns;
}
