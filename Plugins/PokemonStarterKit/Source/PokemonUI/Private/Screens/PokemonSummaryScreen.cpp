// "Unreal Pokémon" created by Retro & Chill.

#include "Screens/PokemonSummaryScreen.h"
#include "Components/Image.h"
#include "Components/Summary/HoldItemInfo.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Components/Summary/SummaryScreenPage.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Utilities/WidgetUtilities.h"
#include "Components/Summary/SummaryPages.h"

void UPokemonSummaryScreen::NativeConstruct() {
    Super::NativeConstruct();

    SummaryPages->GetOnPokemonChange().BindUObject(this, &UPokemonSummaryScreen::SetPokemon);
    SummaryPages->GetOnSelected().AddUObject(this, &UPokemonSummaryScreen::SummaryPageConfirm);
    SummaryPages->GetOnScreenBackOut().AddUObject(this, &UPokemonSummaryScreen::CloseScreen);
    SummaryPages->ActivateWidget();
    SummaryPages->SetPage(0);
}

void UPokemonSummaryScreen::SetInitialPokemon(const TScriptInterface<ITrainer> &Trainer, int32 InitialIndex) {
    SetInitialPokemon(Trainer->GetParty(), InitialIndex);
}

void UPokemonSummaryScreen::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex) {
    SummaryPages->SetInitialPokemon(Party, InitialIndex);
}

USummaryPages *UPokemonSummaryScreen::GetSummaryPages() const {
    return SummaryPages;
}

void UPokemonSummaryScreen::SetPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    NameInfo->Refresh(Pokemon);
    ItemInfo->Refresh(Pokemon);
    SummaryPages->Refresh(Pokemon);

    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto [SpriteMaterial, Size] = GraphicsLoadingSubsystem->GetPokemonUISprite(Pokemon, this);
    UWidgetUtilities::SetBrushFromAsset(PokemonSprite, SpriteMaterial);
    PokemonSprite->SetDesiredSizeOverride(Size);
}

void UPokemonSummaryScreen::SummaryPageConfirm(int32 Index) {
    auto Page = SummaryPages->GetPage(Index);
    if (Page == nullptr || !Page->CanSelectOnPage()) {
        return;
    }

    Page->GetPageSelected().Broadcast();
}