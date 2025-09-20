// "Unreal Pokémon" created by Retro & Chill.

#include "Components/InfoRow.h"

void UInfoRow::NativePreConstruct()
{
    Super::NativePreConstruct();
    UpdateVisibility();
}

void UInfoRow::UpdateVisibility()
{
#if WITH_EDITOR
    if (IsDesignTime())
    {
        SetVisibility(InitialVisibility);
        return;
    }
#endif

    if (!InitialVisibilityDelegate.IsBound())
    {
        SetVisibility(InitialVisibility);
        return;
    }

    SetVisibility(InitialVisibilityDelegate.Execute());
}