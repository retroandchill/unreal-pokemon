// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessageWithChoices.h"
#include "RPGMenusSubsystem.h"
#include "RPGPlayerController.h"
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
    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    auto Screen = Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->ConditionallyAddScreenToStack(ScreenClass);
    Screen->DisplayChoices(Message, Choices);
    Screen->ProcessChoice.AddDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}

void UDisplayMessageWithChoices::ExecuteOnChoiceSelected(int32 ChoiceIndex, FName ChoiceID) {
    OnChoiceSelected.Broadcast(ChoiceIndex, ChoiceID);
    auto Controller = WorldContextObject->GetWorld()->GetFirstPlayerController();
    auto Screen = Controller->GetLocalPlayer()->GetSubsystem<URPGMenusSubsystem>()->GetTopOfStack<UTextDisplayScreen>();
    if (Screen == nullptr)
        return;

    Screen->ProcessChoice.RemoveDynamic(this, &UDisplayMessageWithChoices::ExecuteOnChoiceSelected);
}
