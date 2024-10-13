// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "PokemonUISettings.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"

UDisplayMessage *UDisplayMessage::DisplayMessage(const UObject *WorldContextObject, FText Message) {
    auto Node = NewObject<UDisplayMessage>();
    Node->WorldContextObject = WorldContextObject;
    Node->Message = MoveTemp(Message);
    return Node;
}

void UDisplayMessage::Activate() {
    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    if (Controller == nullptr) {
        // TODO: Remove this hack and alter how the tests run to avoid this
        OnConfirm.Broadcast();
        return;
    }
    
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr) {
        Screen = UTextDisplayScreen::AddTextDisplayScreenToOverlay(WorldContextObject);
    }
    Screen->SetText(Message);
    Screen->NextMessage.AddUniqueDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}

void UDisplayMessage::ExecuteOnConfirm() {
    OnConfirm.Broadcast();
    OnConfirm.Clear();

    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
        return;

    Screen->NextMessage.RemoveDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
    SetReadyToDestroy();
}
