// "Unreal Pokémon" created by Retro & Chill.

#include "Components/RPGButtonBase.h"

URPGButtonBase::URPGButtonBase() {
    SetIsSelectable(true);
}

void URPGButtonBase::NativePreConstruct() {
    Super::NativePreConstruct();
    UpdateVisibility();
}

void URPGButtonBase::UpdateVisibility() {
#if WITH_EDITOR
    if (IsDesignTime()) {
        return;
    }
#endif

    if ((!bIsVisible && !bIsVisibleDelegate.IsBound()) ||
        (bIsVisibleDelegate.IsBound() && !bIsVisibleDelegate.Execute())) {
        SetVisibility(DisabledVisibility);
    } else {
        SetVisibility(EnabledVisibility);
    }
}