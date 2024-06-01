// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"
#include "RPGMenusSubsystem.h"
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
    auto Screen =
        Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->ConditionallyAddScreenToStack(ScreenClass);
    Screen->SetText(Message);
    Screen->NextMessage.AddDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}

void UDisplayMessage::ExecuteOnConfirm() {
    OnConfirm.Broadcast();

    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    auto Screen = Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->GetTopOfStack<UTextDisplayScreen>();
    if (Screen == nullptr)
        return;

    Screen->NextMessage.RemoveDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}
