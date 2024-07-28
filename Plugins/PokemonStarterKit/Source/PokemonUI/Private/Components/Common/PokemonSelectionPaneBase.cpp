// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Party/PokemonPanel.h"
#include "Components/Party/SelectablePanel.h"
#include <range/v3/algorithm/for_each.hpp>

void UPokemonSelectionPaneBase::SetPokemonToDisplay(TConstArrayView<TScriptInterface<IPokemon>> Pokemon) {
    ClearSelectableOptions();
    int32 MaxPartySize = GetNumPanelsToAdd().Get(Pokemon.Num());
    for (int32 i = 0; i < MaxPartySize; i++) {
        if (i < Pokemon.Num()) {
            auto Name = FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}}));
            auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass, FName(Name));
            NewWidget->SetOwner(this);
            NewWidget->SetPokemon(Pokemon[i], i);
            SlotOption(NewWidget);
        } else {
            check(BlankPanelClass != nullptr)
            auto NewWidget = WidgetTree->ConstructWidget<UWidget>(BlankPanelClass);
            PlaceOptionIntoWidget(NewWidget, i);
        }
    }
}

void UPokemonSelectionPaneBase::RefreshWindow() {
    ranges::for_each(GetSelectableOptions<ISelectablePanel>(), &ISelectablePanel::Refresh);
}

bool UPokemonSelectionPaneBase::IsSwitching() const {
    return SwitchingIndex.IsSet();
}

const TOptional<int32> &UPokemonSelectionPaneBase::GetSwitchingIndex() const {
    return SwitchingIndex;
}

void UPokemonSelectionPaneBase::BeginSwitch(int32 StartIndex) {
    check(!SwitchingIndex.IsSet())
    SwitchingIndex.Emplace(StartIndex);
    GetSelectableOption<ISelectablePanel>(StartIndex)->Refresh();
}

void UPokemonSelectionPaneBase::CompleteSwitch() {
    check(SwitchingIndex.IsSet())
    auto Panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.GetValue());
    auto Panel2 = GetSelectableOption<UPokemonPanel>(GetIndex());
    SwitchingIndex.Reset();
    PerformSwap(Panel1, Panel2);
}

void UPokemonSelectionPaneBase::CancelSwitch() {
    check(SwitchingIndex.IsSet())
    auto Panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.GetValue());
    auto Panel2 = GetSelectableOption<UPokemonPanel>(GetIndex());
    SwitchingIndex.Reset();
    Panel1->Refresh();
    Panel2->Refresh();
}

TOptional<int32> UPokemonSelectionPaneBase::GetNumPanelsToAdd() const {
    return TOptional<int32>();
}

void UPokemonSelectionPaneBase::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    RefreshWindow();
    if (NewIndex != INDEX_NONE) {
        OnPokemonSelected.Broadcast(GetSelectableOption<UPokemonPanel>(NewIndex)->GetPokemon());
    }
}

void UPokemonSelectionPaneBase::PerformSwap_Implementation(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    SwitchPokemon(Panel1, Panel2);
}

void UPokemonSelectionPaneBase::SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    Panel1->SwapPokemon(*Panel2);
    Panel1->Refresh();
    Panel2->Refresh();

    OnPokemonSelected.Broadcast(GetSelectableOption<UPokemonPanel>(GetIndex())->GetPokemon());
}