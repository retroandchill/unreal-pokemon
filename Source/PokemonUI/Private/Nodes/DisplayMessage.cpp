// Created by Retro & Chill.


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

	auto Screen = Controller->AddScreenToStack(ScreenClass);
	Screen->SetText(Message);
	Screen->NextMessage.AddDynamic(this, &UDisplayMessage::ExecuteOnConfirm);
}

void UDisplayMessage::ExecuteOnConfirm() {
	if (auto Controller = Cast<ARPGPlayerController>(WorldContextObject->GetWorld()->GetFirstPlayerController()); Controller != nullptr) {
		Controller->RemoveScreenFromStack();
	}
	
	OnConfirm.Broadcast();
}
