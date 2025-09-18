// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/InteractiveButtonGroup.h"

void UInteractiveButtonGroup::OnWidgetAdded(UWidget *Widget)
{
    Super::OnWidgetAdded(Widget);

    const int32 Index = Buttons.Num() - 1;
    NativeOnAddButton.Broadcast(Index, CastChecked<UCommonButtonBase>(Widget));
    OnAddButton.Broadcast(Index, CastChecked<UCommonButtonBase>(Widget));
}

void UInteractiveButtonGroup::OnWidgetRemoved(UWidget *Widget)
{
    Super::OnWidgetRemoved(Widget);

    const auto Button = CastChecked<UCommonButtonBase>(Widget);
    NativeOnRemoveButton.Broadcast(Button);
    OnRemoveButton.Broadcast(Button);
}
