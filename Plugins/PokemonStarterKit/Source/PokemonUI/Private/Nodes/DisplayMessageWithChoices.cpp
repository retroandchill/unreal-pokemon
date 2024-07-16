// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessageWithChoices.h"
#include "PrimaryGameLayout.h"
#include "Screens/TextDisplayScreen.h"

UDisplayMessageWithChoices *
UDisplayMessageWithChoices::DisplayMessageWithChoices(const UObject *WorldContextObject,
                                                      TSubclassOf<UTextDisplayScreen> ScreenClass, FText Message,
                                                      const TArray<FText> &Choices) {
    auto Node = NewObject<UDisplayMessageWithChoices>();
    Node->WorldContextObject = WorldContextObject;
    Node->ScreenClass = ScreenClass;
    Node->Message = Message;
    Node->Choices = Choices;
    return Node;
}

void UDisplayMessageWithChoices::Activate() {
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr) {
        Screen = Layout->PushWidgetToLayerStack<UTextDisplayScreen>(RPG::Menus::OverlayMenuLayerTag, ScreenClass);
    }
    Screen->DisplayChoices(Message, Choices);
    Screen->ProcessChoice.AddUniqueDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}

void UDisplayMessageWithChoices::ExecuteOnChoiceSelected(int32 ChoiceIndex, FName ChoiceID) {
    OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);
    auto Layout = UPrimaryGameLayout::GetPrimaryGameLayoutForPrimaryPlayer(WorldContextObject);
    auto Screen = Cast<UTextDisplayScreen>(Layout->GetLayerWidget(RPG::Menus::OverlayMenuLayerTag)->GetActiveWidget());
    if (Screen == nullptr)
        return;

    Screen->ProcessChoice.RemoveDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
    SetReadyToDestroy();
}
