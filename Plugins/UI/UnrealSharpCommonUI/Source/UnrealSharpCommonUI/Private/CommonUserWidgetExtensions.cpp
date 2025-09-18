// "Unreal Pokémon" created by Retro & Chill.

#include "CommonUserWidgetExtensions.h"
#include "CommonActivatableWidget.h"
#include "Input/CSBindUIActionArgs.h"

TArray<FUIActionBindingHandle> UCommonUserWidgetExtensions::GetActionBindings(const UCommonUserWidget *Widget)
{
    return Widget->GetActionBindings();
}

FUIActionBindingHandle UCommonUserWidgetExtensions::RegisterUIActionBinding(UCommonUserWidget *Widget,
                                                                            const FCSBindUIActionArgs &BindActionArgs)
{
    return Widget->RegisterUIActionBinding(*BindActionArgs.GetArgs());
}

void UCommonUserWidgetExtensions::AddActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs)
{
    Widget->AddActionBinding(MoveTemp(BindActionArgs));
}

void UCommonUserWidgetExtensions::RemoveActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs)
{
    Widget->RemoveActionBinding(MoveTemp(BindActionArgs));
}