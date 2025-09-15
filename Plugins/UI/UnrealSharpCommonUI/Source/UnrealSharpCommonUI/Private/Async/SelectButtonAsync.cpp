// Fill out your copyright notice in the Description page of Project Settings.

#include "Async/SelectButtonAsync.h"
#include "Groups/CommonButtonGroupBase.h"

void USelectButtonAsync::SelectButton(UCommonButtonGroupBase *InButtonGroup)
{
    ButtonGroup = InButtonGroup;
    DelegateHandle = ButtonGroup->NativeOnButtonBaseClicked.AddUObject(this, &USelectButtonAsync::OnSelectButton);
}

void USelectButtonAsync::OnSelectButton(UCommonButtonBase *Button, const int32 Index)
{
    ButtonGroup->NativeOnButtonBaseClicked.Remove(DelegateHandle);
    SelectedButton = Button;
    SelectedButtonIndex = Index;
    InvokeManagedCallback();
}

void USelectButtonAsync::CancelSelection()
{
    bCancelled = true;
    ButtonGroup->NativeOnButtonBaseClicked.Remove(DelegateHandle);
    InvokeManagedCallback();
}
