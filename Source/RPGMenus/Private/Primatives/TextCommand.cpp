// Created by Retro & Chill.


#include "Primatives/TextCommand.h"

#include "Components/TextBlock.h"

UTextCommand::UTextCommand(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer) {
}

TSharedRef<SWidget> UTextCommand::RebuildWidget() {
	auto Ret = Super::RebuildWidget();

	if (DisplayTextWidget != nullptr) {
		DisplayTextWidget->SetText(DisplayText);
	}
	
	return Ret;
}

void UTextCommand::Select() {
	CommandDelegate.Broadcast();
}
