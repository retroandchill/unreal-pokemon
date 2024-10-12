// "Unreal Pokémon" created by Retro & Chill.

#include "Nodes/Moves/PromptReplaceMove.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/MoveForgetScreen.h"

UPromptReplaceMove *UPromptReplaceMove::PromptReplaceMove(const UObject *WorldContextObject,
                                                          const TScriptInterface<IPokemon> &Pokemon, FName Move) {
    auto Node = NewObject<UPromptReplaceMove>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

void UPromptReplaceMove::Activate() {
    auto &ScreenPaths = *GetDefault<UPokemonUISettings>();
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Layout->PushWidgetToLayerStack<UMoveForgetScreen>(
        RPG::Menus::PrimaryMenuLayerTag, ScreenPaths.MoveForgetScreenClass.TryLoadClass<UMoveForgetScreen>());
    Screen->InitializeScene(Pokemon, Move);
    Screen->BindToOnMoveForgetComplete(this, &UPromptReplaceMove::OnMoveSelectionComplete);
}

void UPromptReplaceMove::OnMoveSelectionComplete(bool bMoveReplaced) {
    if (bMoveReplaced) {
        MoveLearned.Broadcast();
    } else {
        MoveNotLearned.Broadcast();
    }
    SetReadyToDestroy();
}