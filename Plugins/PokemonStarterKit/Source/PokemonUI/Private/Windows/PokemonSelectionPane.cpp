// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/PokemonSelectionPane.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Party/PartySelectCancelPanel.h"
#include "Components/Party/PokemonPanel.h"
#include "Managers/PokemonSubsystem.h"
#include "PokemonDataSettings.h"
#include <range/v3/algorithm/for_each.hpp>

void UPokemonSelectionPane::NativeConstruct() {
    Super::NativeConstruct();

    const auto &PokemonSubsystem = UPokemonSubsystem::GetInstance(this);
    auto &PlayerParty = PokemonSubsystem.GetPlayer()->GetParty();

    ClearSelectableOptions();
    auto MaxPartySize = GetDefault<UPokemonDataSettings>()->MaxPartySize;
    for (int32 i = 0; i < MaxPartySize; i++) {
        if (i < PlayerParty.Num()) {
            auto Name = FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}}));
            auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass, FName(Name));
            NewWidget->SetOwner(this);
            NewWidget->SetPokemon(PlayerParty[i], i);
            SlotOption(NewWidget);
        } else {
            auto NewWidget = WidgetTree->ConstructWidget<UWidget>(BlankPanelClass);
            PlaceOptionIntoWidget(NewWidget, i);
        }
    }
}

void UPokemonSelectionPane::RefreshWindow() {
    ranges::for_each(GetSelectableOptions<ISelectablePanel>(), &ISelectablePanel::Refresh);
}

bool UPokemonSelectionPane::IsSwitching() const {
    return SwitchingIndex.IsSet();
}

const TOptional<int32> &UPokemonSelectionPane::GetSwitchingIndex() const {
    return SwitchingIndex;
}

void UPokemonSelectionPane::BeginSwitch(int32 StartIndex) {
    check(!SwitchingIndex.IsSet())
    SwitchingIndex.Emplace(StartIndex);
    GetSelectableOption<ISelectablePanel>(StartIndex)->Refresh();
}

void UPokemonSelectionPane::CompleteSwitch() {
    check(SwitchingIndex.IsSet())
    auto Panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.GetValue());
    auto Panel2 = GetSelectableOption<UPokemonPanel>(GetIndex());
    SwitchingIndex.Reset();
    PerformSwap(Panel1, Panel2);
}

void UPokemonSelectionPane::CancelSwitch() {
    check(SwitchingIndex.IsSet())
    auto Panel1 = GetSelectableOption<UPokemonPanel>(SwitchingIndex.GetValue());
    auto Panel2 = GetSelectableOption<UPokemonPanel>(GetIndex());
    SwitchingIndex.Reset();
    Panel1->Refresh();
    Panel2->Refresh();
}

void UPokemonSelectionPane::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    RefreshWindow();
    OnPokemonSelected.Broadcast(GetSelectableOption<UPokemonPanel>(NewIndex)->GetPokemon());
}

void UPokemonSelectionPane::PerformSwap_Implementation(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    SwitchPokemon(Panel1, Panel2);
}

void UPokemonSelectionPane::SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    Panel1->SwapPokemon(*Panel2);
    Panel1->Refresh();
    Panel2->Refresh();
    
    OnPokemonSelected.Broadcast(GetSelectableOption<UPokemonPanel>(GetIndex())->GetPokemon());
}

void UPokemonSelectionPane::AddAdditionalPanelToOptions(UPartySelectCancelPanel *Panel) {
    Panel->Refresh();
    SlotOption(Panel);
}
