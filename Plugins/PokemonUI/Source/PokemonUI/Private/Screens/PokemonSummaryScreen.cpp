// "Unreal Pokémon" created by Retro & Chill.


#include "Screens/PokemonSummaryScreen.h"
#include "Components/Image.h"
#include "Components/Summary/HoldItemInfo.h"
#include "Components/Summary/SummaryNameInfo.h"
#include "Graphics/GraphicsLoadingSubsystem.h"
#include "Utilities/WidgetUtilities.h"
#include "Windows/SummaryPages.h"

void UPokemonSummaryScreen::NativeConstruct() {
    Super::NativeConstruct();

    SummaryPages->GetOnPokemonChange().BindUObject(this, &UPokemonSummaryScreen::SetPokemon);
    SummaryPages->GetOnCancel().AddDynamic(this, &UPokemonSummaryScreen::CloseScreen);
    SummaryPages->SetActive(true);
    SummaryPages->SetIndex(0);
}

void UPokemonSummaryScreen::SetInitialPokemon(TConstArrayView<TScriptInterface<IPokemon>> Party, int32 InitialIndex) {
    SummaryPages->SetInitialPokemon(Party, InitialIndex);
}

void UPokemonSummaryScreen::SetPokemon(const TScriptInterface<IPokemon> &Pokemon) {
    NameInfo->Refresh(Pokemon);
    ItemInfo->Refresh(Pokemon);
    SummaryPages->Refresh(Pokemon);

    auto GraphicsLoadingSubsystem = GetGameInstance()->GetSubsystem<UGraphicsLoadingSubsystem>();
    auto [SpriteMaterial, Size] = GraphicsLoadingSubsystem->GetPokemonUISprite(*Pokemon, this);
    UWidgetUtilities::SetBrushFromAsset(PokemonSprite, SpriteMaterial);
    PokemonSprite->SetDesiredSizeOverride(Size);
}