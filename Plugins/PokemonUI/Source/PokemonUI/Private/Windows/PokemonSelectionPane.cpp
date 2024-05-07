// "Unreal Pokémon" created by Retro & Chill.
#include "Windows/PokemonSelectionPane.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/WidgetSwitcher.h"
#include "Managers/PokemonSubsystem.h"
#include "Components/PartySelectCancelPanel.h"
#include "Components/PokemonPanel.h"

void UPokemonSelectionPane::NativeConstruct() {
    Super::NativeConstruct();

    check(ContentsArea != nullptr)
    const auto &PokemonSubsystem = UPokemonSubsystem::GetInstance();
    auto &PlayerParty = PokemonSubsystem.GetPlayer()->GetParty();

    ActivePanels.Empty();
    for (int32 i = 0; i < PokemonSubsystem.GetMaxPartySize(); i++) {
        if (i < PlayerParty.Num()) {
            auto Name = FString::Format(TEXT("SelectionPanel{Num}"), FStringFormatNamedArguments({{TEXT("Num"), i}}));
            auto NewWidget = WidgetTree->ConstructWidget<UPokemonPanel>(PanelClass, FName(Name));
            NewWidget->SetOwner(this);
            NewWidget->SetPokemon(PlayerParty[i], i);
            NewWidget->GetOnOptionClicked().AddDynamic(this, &UPokemonSelectionPane::ProcessClickedButton);
            NewWidget->GetOnOptionHovered().AddDynamic(this, &UPokemonSelectionPane::ProcessHoveredButton);
            auto PanelSlot = ContentsArea->AddChildToCanvas(NewWidget);
            auto [Offsets, Anchors] = GetPanelOffset(i);
            PanelSlot->SetAnchors(Anchors);
            PanelSlot->SetOffsets(Offsets);
            ActivePanels.Add(NewWidget);
        } else {
            auto NewWidget = WidgetTree->ConstructWidget<UWidget>(BlankPanelClass);
            auto PanelSlot = ContentsArea->AddChildToCanvas(NewWidget);
            auto [Offsets, Anchors] = GetPanelOffset(i);
            PanelSlot->SetAnchors(Anchors);
            PanelSlot->SetOffsets(Offsets);
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

bool UPokemonSelectionPane::IsSwitching() const {
    return SwitchingIndex.IsSet();
}

const TOptional<int32> &UPokemonSelectionPane::GetSwitchingIndex() const {
    return SwitchingIndex;
}

void UPokemonSelectionPane::BeginSwitch(int32 StartIndex) {
    check(!SwitchingIndex.IsSet())
    SwitchingIndex.Emplace(StartIndex);
    ActivePanels[StartIndex]->Refresh();
}

void UPokemonSelectionPane::CompleteSwitch() {
    check(SwitchingIndex.IsSet())
    auto Panel1 = CastChecked<UPokemonPanel>(ActivePanels[SwitchingIndex.GetValue()].GetObject());
    auto Panel2 = CastChecked<UPokemonPanel>(ActivePanels[GetIndex()].GetObject());
    SwitchingIndex.Reset();
    PerformSwap(Panel1, Panel2);
}

TPair<FMargin, FAnchors> UPokemonSelectionPane::GetPanelOffset(int32 PanelIndex) {
    TPair<FMargin, FAnchors> Ret;
    GetPanelOffset(PanelIndex, Ret.Key, Ret.Value);
    return Ret;
}

void UPokemonSelectionPane::OnSelectionChange_Implementation(int32 OldIndex, int32 NewIndex) {
    for (const auto &Panel : ActivePanels) {
        Panel->Refresh();
    }
}

void UPokemonSelectionPane::PerformSwap_Implementation(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    SwitchPokemon(Panel1, Panel2);
}

void UPokemonSelectionPane::SwitchPokemon(UPokemonPanel *Panel1, UPokemonPanel *Panel2) {
    Panel1->SwapPokemon(*Panel2);
    Panel1->Refresh();
    Panel2->Refresh();
}

void UPokemonSelectionPane::AddAdditionalPanelToOptions(TObjectPtr<UPartySelectCancelPanel> &Panel) {
    Panel->SetOptionIndex(ActivePanels.Num());
    Panel->Refresh();
    Panel->GetOnOptionClicked().AddDynamic(this, &UPokemonSelectionPane::ProcessClickedButton);
    Panel->GetOnOptionHovered().AddDynamic(this, &UPokemonSelectionPane::ProcessHoveredButton);
    ActivePanels.Add(Panel);
}
