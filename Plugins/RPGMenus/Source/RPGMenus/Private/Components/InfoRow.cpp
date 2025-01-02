// "Unreal Pokémon" created by Retro & Chill.

#include "Components/InfoRow.h"
#include "RetroLib.h"

void UInfoRow::NativePreConstruct() {
    Super::NativePreConstruct();
    UpdateVisibility();
}

void UInfoRow::UpdateVisibility() {
#if WITH_EDITOR
    if (IsDesignTime()) {
        SetVisibility(InitialVisibility);
        return;
    }
#endif

    // clang-format off
    Retro::Optionals::Of(InitialVisibilityDelegate) |
        Retro::Optionals::Filter<&FGetSlateVisibility::IsBound>() |
        Retro::Optionals::Transform<&FGetSlateVisibility::Execute>() |
        Retro::Optionals::OrElseValue(InitialVisibility);
    // clang-format on
}