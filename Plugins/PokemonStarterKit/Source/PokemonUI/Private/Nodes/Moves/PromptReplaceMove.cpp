// "Unreal Pokémon" created by Retro & Chill.


#include "Nodes/Moves/PromptReplaceMove.h"
#include "RPGMenusSubsystem.h"
#include "Screens/MoveForgetScreen.h"
#include "Settings/BaseSettings.h"

UPromptReplaceMove * UPromptReplaceMove::PromptReplaceMove(const UObject *WorldContextObject, const TScriptInterface<IPokemon> &Pokemon, FName Move) {
    auto Node = NewObject<UPromptReplaceMove>();
    Node->WorldContextObject = WorldContextObject;
    Node->Pokemon = Pokemon;
    Node->Move = Move;
    return Node;
}

void UPromptReplaceMove::Activate() {
    auto &ScreenPaths = Pokemon::FBaseSettings::Get().GetDefaultScreenPaths();
    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    auto Subsystem = Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>();
    auto Screen = Subsystem->AddScreenToStack<UMoveForgetScreen>(ScreenPaths.MoveForgetScreenClass.TryLoadClass<UMoveForgetScreen>());
    Screen->InitializeScene(Pokemon, Move);
    Screen->BindToOnMoveForgetComplete(FOnMoveForgetComplete::FDelegate::CreateUObject(this, &UPromptReplaceMove::OnMoveSelectionComplete));
}

void UPromptReplaceMove::OnMoveSelectionComplete(bool bMoveReplaced) {
    if (bMoveReplaced) {
        MoveLearned.Broadcast();
    } else {
        MoveNotLearned.Broadcast();
    }
}