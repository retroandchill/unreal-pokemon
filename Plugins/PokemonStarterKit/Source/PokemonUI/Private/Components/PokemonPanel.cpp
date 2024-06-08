// "Unreal Pokémon" created by Retro & Chill.

#include "Components/PokemonPanel.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Utilities/PokemonUIUtils.h"
#include "Windows/PokemonSelectionPane.h"
#include "Windows/SelectableWidget.h"

void UPokemonPanel::SetOwner(USelectableWidget *NewOwner) {
    Owner = NewOwner;
}

const TScriptInterface<IPokemon> &UPokemonPanel::GetPokemon() const {
    return Pokemon;
}

void UPokemonPanel::SetPokemon(TScriptInterface<IPokemon> NewPokemon, int32 Index) {
    Pokemon = NewPokemon;
    SetOptionIndex(Index);
    Refresh();
}

void UPokemonPanel::SwapPokemon(UPokemonPanel &Other) {
    Swap(Pokemon, Other.Pokemon);
}

bool UPokemonPanel::IsActive() const {
    return GetOptionIndex() == 0;
}

bool UPokemonPanel::IsPokemonFainted() const {
    return Pokemon != nullptr ? Pokemon->IsFainted() : false;
}

bool UPokemonPanel::IsPanelSelected() const {
    return Owner != nullptr && Owner->GetIndex() == GetOptionIndex();
}

bool UPokemonPanel::IsSwapping() const {
    auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
    if (SelectionPane == nullptr)
        return false;

    return SelectionPane->IsSwitching() && IsPanelSelected();
}

bool UPokemonPanel::IsPreselected() const {
    auto SelectionPane = Cast<UPokemonSelectionPane>(Owner);
    if (SelectionPane == nullptr)
        return false;

    auto &SwitchingIndex = SelectionPane->GetSwitchingIndex();
    return SwitchingIndex.Get(-1) == GetOptionIndex();
}

void UPokemonPanel::Refresh() {
    using enum ESlateVisibility;

    RefreshVisuals();
    if (Pokemon != nullptr) {
        RefreshPokemonInfo();

        auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
        check(GraphicsLoadingSubsystem != nullptr)
        PokemonIcon->SetVisibility(SelfHitTestInvisible);
        PokemonIcon->SetBrushResourceObject(GraphicsLoadingSubsystem->GetPokemonIcon(*Pokemon, this));

        if (auto HoldItem = Pokemon->GetHoldItem(); HoldItem != nullptr) {
            HeldItemIcon->SetVisibility(SelfHitTestInvisible);
            if (auto HeldItemGraphic = GetHeldItemGraphic(*HoldItem); HeldItemGraphic != nullptr) {
                HeldItemIcon->SetBrushResourceObject(HeldItemGraphic);
            }
        } else {
            HeldItemIcon->SetVisibility(Collapsed);
        }
    } else {
        PokemonIcon->SetVisibility(Collapsed);
        HeldItemIcon->SetVisibility(Collapsed);
    }
}

void UPokemonPanel::RefreshPokemonInfo() {
    UPokemonUIUtils::SetItemText(NameText, Pokemon->GetNickname());
    UPokemonUIUtils::SetItemText(LevelText, FString::FromInt(Pokemon->GetStatBlock()->GetLevel()));

    auto Gender = Pokemon->GetGender();
    UPokemonUIUtils::SetPokemonGenderText(*Pokemon, GenderText);
    if (GenderTextColors.Contains(Gender)) {
        UPokemonUIUtils::SetItemTextColor(GenderText, GenderTextColors[Gender]);
    }

    auto HP = FString::Format(
        TEXT("{CurrentHP} /{MaxHP}"),
        FStringFormatNamedArguments({{TEXT("CurrentHP"), UPokemonUIUtils::SpacePad(Pokemon->GetCurrentHP(), 3)},
                                     {TEXT("MaxHP"), UPokemonUIUtils::SpacePad(Pokemon->GetMaxHP(), 3)}}));

    UPokemonUIUtils::SetItemText(HPText, HP);
    UPokemonUIUtils::SetBarValues(HPBar, static_cast<float>(Pokemon->GetCurrentHP()),
                                  static_cast<float>(Pokemon->GetMaxHP()));
}
