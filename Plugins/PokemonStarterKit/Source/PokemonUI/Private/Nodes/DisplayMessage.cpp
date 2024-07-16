// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"

UDisplayMessage *UDisplayMessage::DisplayMessage(const UObject *WorldContextObject,
                                                 TSubclassOf<UTextDisplayScreen> ScreenClass, FText Message) {
    auto Node = NewObject<UDisplayMessage>();
    Node->WorldContextObject = WorldContextObject;
    Node->ScreenClass = ScreenClass;
    Node->Message = Message;
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
        Screen = Layout->PushWidgetToLayerStack<UTextDisplayScreen>(RPG::Menus::OverlayMenuLayerTag, ScreenClass);
    }
    Screen->SetText(Message);
    Screen->NextMessage.AddUniqueDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}

void UDisplayMessage::ExecuteOnConfirm() {
    OnConfirm.Broadcast();

    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
        return;

    Screen->NextMessage.RemoveDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}
