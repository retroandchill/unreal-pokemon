// Fill out your copyright notice in the Description page of Project Settings.

#include "Input/UIActionBindingExtensions.h"
#include "Input/UIActionBindingHandle.h"

bool UUIActionBindingExtensions::IsValid(FUIActionBindingHandle Handle)
{
    return Handle.IsValid();
}

void UUIActionBindingExtensions::Unregister(FUIActionBindingHandle Handle)
{
    Handle.Unregister();
}

void UUIActionBindingExtensions::ResetHold(FUIActionBindingHandle Handle)
{
    Handle.ResetHold();
}

// ReSharper disable once CppPassValueParameterByConstReference
FName UUIActionBindingExtensions::GetActionName(FUIActionBindingHandle Handle)
{
    return Handle.GetActionName();
}

// ReSharper disable once CppPassValueParameterByConstReference
FText UUIActionBindingExtensions::GetDisplayName(FUIActionBindingHandle Handle)
{
    return Handle.GetDisplayName();
}

void UUIActionBindingExtensions::SetDisplayName(FUIActionBindingHandle Handle, const FText &DisplayName)
{
    return Handle.SetDisplayName(DisplayName);
}

// ReSharper disable once CppPassValueParameterByConstReference
bool UUIActionBindingExtensions::GetDisplayInActionBar(FUIActionBindingHandle Handle)
{
    return Handle.GetDisplayInActionBar();
}

void UUIActionBindingExtensions::SetDisplayInActionBar(FUIActionBindingHandle Handle, const bool bDisplayInActionBar)
{
    Handle.SetDisplayInActionBar(bDisplayInActionBar);
}

// ReSharper disable once CppPassValueParameterByConstReference
const UWidget *UUIActionBindingExtensions::GetBoundWidget(FUIActionBindingHandle Handle)
{
    return Handle.GetBoundWidget();
}

// ReSharper disable once CppPassValueParameterByConstReference
ULocalPlayer *UUIActionBindingExtensions::GetBoundLocalPlayer(FUIActionBindingHandle Handle)
{
    return Handle.GetBoundLocalPlayer();
}
