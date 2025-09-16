// "Unreal Pokémon" created by Retro & Chill.

#include "Components/InteractiveButtonBase.h"

UInteractiveButtonBase::UInteractiveButtonBase()
{
    SetIsSelectable(true);
}

void UInteractiveButtonBase::NativePreConstruct()
{
    Super::NativePreConstruct();
    UpdateVisibility();
}

void UInteractiveButtonBase::UpdateVisibility()
{
#if WITH_EDITOR
    if (IsDesignTime())
    {
        return;
    }
#endif

    if ((!bIsVisible && !bIsVisibleDelegate.IsBound()) ||
        (bIsVisibleDelegate.IsBound() && !bIsVisibleDelegate.Execute()))
    {
        SetVisibility(DisabledVisibility);
    }
    else
    {
        SetVisibility(EnabledVisibility);
    }
}