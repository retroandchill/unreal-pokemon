// "Unreal Pokémon" created by Retro & Chill.


#include "Components/InfoRow.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/Map.h"
#include "Ranges/Optional/Of.h"
#include "Ranges/Optional/OrElse.h"

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
    UE::Optionals::Of(InitialVisibilityDelegate) |
        UE::Optionals::Filter(&FGetSlateVisibility::IsBound) |
        UE::Optionals::Map(&FGetSlateVisibility::Execute) |
        UE::Optionals::OrElse(InitialVisibility);
    // clang-format on
}