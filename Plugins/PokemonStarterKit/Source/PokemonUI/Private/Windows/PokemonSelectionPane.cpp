// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/PokemonSelectionPane.h"
#include "Blueprint/WidgetTree.h"
#include "Components/PartySelectCancelPanel.h"
#include "Components/PokemonPanel.h"
#include "Managers/PokemonSubsystem.h"
#include "Settings/BaseSettings.h"
#include <range/v3/algorithm/for_each.hpp>

void UPokemonSelectionPane::NativeConstruct() {
    Super::NativeConstruct();

    const auto &PokemonSubsystem = UPokemonSubsystem::GetInstance(this);
    auto &PlayerParty = PokemonSubsystem.GetPlayer()->GetParty();

    ClearSelectableOptions();
    for (int32 i = 0; i < Pokemon::FBaseSettings::Get().GetMaxPartySize(); i++) {
        if (i < PlayerParty.Num()) {
            auto Name = FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}}));
            auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass, FName(Name));
            NewWidget->SetOwner(this);
            NewWidget->SetPokemon(PlayerParty[i], i);
            SlotOption(NewWidget, i);
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

void UPokemonSelectionPane::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    RefreshWindow();
}

void UPokemonSelectionPane::PerformSwap_Implementation(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    SwitchPokemon(Panel1, Panel2);
}

void UPokemonSelectionPane::SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    Panel1->SwapPokemon(*Panel2);
    Panel1->Refresh();
    Panel2->Refresh();
}

void UPokemonSelectionPane::AddAdditionalPanelToOptions(UPartySelectCancelPanel* Panel) {
    Panel->Refresh();
    SlotOption(Panel, GetItemCount());
}
