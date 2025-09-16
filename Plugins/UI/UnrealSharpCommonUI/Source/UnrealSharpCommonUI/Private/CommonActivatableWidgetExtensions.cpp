// "Unreal Pokémon" created by Retro & Chill.


#include "CommonActivatableWidgetExtensions.h"
#include "CommonActivatableWidget.h"
#include "Input/CSBindUIActionArgs.h"

TArray<FUIActionBindingHandle> UCommonActivatableWidgetExtensions::GetActionBindings(const UCommonActivatableWidget *Widget)
{
    return Widget->GetActionBindings();
}

FUIActionBindingHandle UCommonActivatableWidgetExtensions::RegisterUIActionBinding(UCommonActivatableWidget *Widget,
    const FCSBindUIActionArgs &BindActionArgs)
{
    return Widget->RegisterUIActionBinding(*BindActionArgs.GetArgs());
}

void UCommonActivatableWidgetExtensions::AddActionBinding(UCommonActivatableWidget *Widget,
    FUIActionBindingHandle BindActionArgs)
{
    Widget->AddActionBinding(MoveTemp(BindActionArgs));   
}

void UCommonActivatableWidgetExtensions::RemoveActionBinding(UCommonActivatableWidget *Widget,
    FUIActionBindingHandle BindActionArgs)
{
    Widget->RemoveActionBinding(MoveTemp(BindActionArgs));   
}