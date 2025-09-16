// Fill out your copyright notice in the Description page of Project Settings.


#include "Async/PlayDialogueBoxTextAsync.h"

#include "Components/DialogueBox.h"

void UPlayDialogueBoxTextAsync::PlayDialogueBoxText(UDialogueBox* DialogueBox, const FText& Text)
{
    Widget = DialogueBox;
    DelegateHandle = Widget->BindToOnLineFinishedPlaying(FSimpleDelegate::CreateUObject(this, &UPlayDialogueBoxTextAsync::OnAsyncLoadComplete));
    Widget->PlayLine(Text);
}

void UPlayDialogueBoxTextAsync::OnAsyncLoadComplete()
{
    Widget->UnbindFromOnLineFinishedPlaying(DelegateHandle);
    InvokeManagedCallback();
}
