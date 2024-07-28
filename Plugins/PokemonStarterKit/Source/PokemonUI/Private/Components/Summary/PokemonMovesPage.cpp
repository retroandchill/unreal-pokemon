// "Unreal Pokémon" created by Retro & Chill.

#include "Components/Summary/PokemonMovesPage.h"
#include "Input/CommonUIInputTypes.h"

void UPokemonMovesPage::NativeConstruct() {
    Super::NativeConstruct();

    FBindUIActionArgs BindArgs(SelectionAction, FSimpleDelegate::CreateWeakLambda(this, [this] {
                                   if (bIsShown) {
                                       OnSelected.Broadcast();
                                   }
                               }));
    BindArgs.bDisplayInActionBar = false;
    BindArgs.OverrideDisplayName = ActionText;
    InspectActionHandler = RegisterUIActionBinding(BindArgs);
}

void UPokemonMovesPage::RefreshInfo_Implementation(const TScriptInterface<IPokemon> &Pokemon) {
    Super::RefreshInfo_Implementation(Pokemon);
    DrawPokemonMoves.Broadcast(Pokemon);
}

void UPokemonMovesPage::OnPageShown() {
    bIsShown = true;
    InspectActionHandler.SetDisplayInActionBar(true);
}

void UPokemonMovesPage::OnPageHidden() {
    bIsShown = false;
    InspectActionHandler.SetDisplayInActionBar(false);
}