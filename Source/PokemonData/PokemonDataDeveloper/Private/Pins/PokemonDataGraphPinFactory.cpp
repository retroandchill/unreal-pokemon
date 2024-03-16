// "Unreal Pokémon" created by Retro & Chill.


#include "Pins/PokemonDataGraphPinFactory.h"

#include "SGraphPinNameList.h"
#include "Nodes/SelectDataRow.h"

TSharedPtr<SGraphPin> FPokemonDataGraphPinFactory::CreatePin(UEdGraphPin* Pin) const {
	const auto* const Outer = Cast<ISelectDataRow>(Pin->GetOuter());
	if (Pin->PinType.PinCategory != UEdGraphSchema_K2::PC_Name || Outer == nullptr || Pin != Outer->GetRowPin()) {
		return FGraphPanelPinFactory::CreatePin(Pin);
	}

	auto RowNames = Outer->GetRowNames();
	if (RowNames.IsEmpty()) {
		return FGraphPanelPinFactory::CreatePin(Pin);
	}

	TArray<TSharedPtr<FName>> NamesList;
	for (auto Name : RowNames) {
		NamesList.Add(MakeShared<FName>(Name));
	}
	
	return SNew(SGraphPinNameList, Pin, NamesList);
}
