// "Unreal Pokémon" created by Retro & Chill.


#include "Buttons/PokemonInfoButtonBase.h"
#include "CommonNumericTextBlock.h"
#include "CommonTextBlock.h"
#include "Components/Image.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Pokemon/Pokemon.h"
#include "Pokemon/Stats/StatBlock.h"
#include "Utilities/PokemonUIUtils.h"

class UGraphicsLoadingSubsystem;

const TScriptInterface<IPokemon> & UPokemonInfoButtonBase::GetPokemon() const {
    return Pokemon;
}

int32 UPokemonInfoButtonBase::GetPartyIndex() const {
    return PokemonIndex;
}

void UPokemonInfoButtonBase::SetPokemon(TScriptInterface<IPokemon> NewPokemon, int32 Index) {
    Pokemon = NewPokemon;
    PokemonIndex = Index;
    Refresh();
}

void UPokemonInfoButtonBase::SwapPokemon(UPokemonInfoButtonBase &Other) {
    Swap(Pokemon, Other.Pokemon);
}

bool UPokemonInfoButtonBase::IsActivated() const {
    return PokemonIndex == 1;
}

bool UPokemonInfoButtonBase::IsPokemonFainted() const {
    return Pokemon->IsFainted();
}

bool UPokemonInfoButtonBase::IsSwapping() const {
    return false;
}

void UPokemonInfoButtonBase::Refresh() {
    using enum ESlateVisibility;

    RefreshVisuals();
    if (Pokemon != nullptr) {
        RefreshPokemonInfo();

        auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
        check(GraphicsLoadingSubsystem != nullptr)
        PokemonIcon->SetVisibility(SelfHitTestInvisible);
        PokemonIcon->SetBrushResourceObject(GraphicsLoadingSubsystem->GetPokemonIcon(Pokemon, this));

        if (auto HoldItem = Pokemon->GetHoldItem(); HoldItem != nullptr) {
            HeldItemIcon->SetVisibility(SelfHitTestInvisible);
            if (auto HeldItemGraphic = GetHeldItemGraphic(HoldItem->ID); HeldItemGraphic != nullptr) {
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

void UPokemonInfoButtonBase::RefreshPokemonInfo() {
    NameText->SetText(Pokemon->GetNickname());
    LevelText->SetCurrentValue(static_cast<float>(Pokemon->GetStatBlock()->GetLevel()));
    SetDisplayedGender(Pokemon->GetGender());

    auto HP = FString::Format(
        TEXT("{CurrentHP} /{MaxHP}"),
        FStringFormatNamedArguments({{TEXT("CurrentHP"), UPokemonUIUtils::SpacePad(Pokemon->GetCurrentHP(), 3)},
                                     {TEXT("MaxHP"), UPokemonUIUtils::SpacePad(Pokemon->GetMaxHP(), 3)}}));

    HPText->SetText(FText::FromString(MoveTemp(HP)));
    UPokemonUIUtils::SetBarValues(HPBar, static_cast<float>(Pokemon->GetCurrentHP()),
                                  static_cast<float>(Pokemon->GetMaxHP()));
}