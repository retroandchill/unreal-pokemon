// "Unreal Pokémon" created by Retro & Chill.
#include "Nodes/DisplayMessage.h"

#include "RPGPlayerController.h"
#include "Screens/TextDisplayScreen.h"

UDisplayMessage* UDisplayMessage::DisplayMessage(const UObject* WorldContextObject,
                                                 TSubclassOf<UTextDisplayScreen> ScreenClass, FText Message) {
	auto Node = NewObject<UDisplayMessage>();
	Node->WorldContextObject = WorldContextObject;
	Node->ScreenClass = ScreenClass;
	Node->Message = Message;
	return Node;
}

void UDisplayMessage::Activate() {
	auto Controller = Cast<ARPGPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController());
	if (Controller == nullptr)
		return;

	auto Screen = Controller->ConditionallyAddScreenToStack(ScreenClass);
	Screen->SetText(Message);
	Screen->NextMessage.AddDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}

void UDisplayMessage::ExecuteOnConfirm() {
	OnConfirm.Broadcast();

	auto Controller = Cast<ARPGPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController());
	if (Controller == nullptr)
		return;

	auto Screen = Controller->GetTopOfStack<UTextDisplayScreen>();
	if (Screen == nullptr)
		return;

	Screen->NextMessage.RemoveDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}
