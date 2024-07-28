// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonSummaryScreen.h"
#include "CommonButtonBase.h"
#include "Components/Image.h"
#include "Components/Common/PokemonSelectionPaneBase.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Utilities/WidgetUtilities.h"
#include "Components/Summary/SummaryPages.h"
#include "Components/Summary/SummaryTabWidget.h"

void UPokemonSummaryScreen::NativePreConstruct() {
    Super::NativePreConstruct();

    if (SummaryTabs != nullptr && SummaryPages != nullptr) {
        SummaryTabs->SetSummaryPages(SummaryPages);
    }
}

void UPokemonSummaryScreen::NativeConstruct() {
    Super::NativeConstruct();
    SummaryPages->GetOnPokemonChange().BindUObject(this, &UPokemonSummaryScreen::SetPokemon);
}

void UPokemonSummaryScreen::SetInitialPokemon(const TScriptInterface<ITrainer> &Trainer, int32 InitialIndex) {
    SetInitialPokemon(Trainer->GetParty(), InitialIndex);
}

void UPokemonSummaryScreen::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex) {
    PokemonSelection->SetPokemonToDisplay(Party);
    PokemonSelection->SetIndex(InitialIndex);
    PokemonSelection->GetSelectedOption()->SetIsSelected(true);
    PokemonSelection->ActivateWidget();
}

USummaryPages *UPokemonSummaryScreen::GetSummaryPages() const {
    return SummaryPages;
}

UPokemonSelectionPaneBase * UPokemonSummaryScreen::GetPokemonSelection() const {
    return PokemonSelection;
}

void UPokemonSummaryScreen::SetPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    NameInfo->Refresh(Pokemon);
    SummaryPages->Refresh(Pokemon);

    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto [SpriteMaterial, Size] = GraphicsLoadingSubsystem->GetPokemonUISprite(Pokemon, this);
    UWidgetUtilities::SetBrushFromAsset(PokemonSprite, SpriteMaterial);
    PokemonSprite->SetDesiredSizeOverride(Size);
}