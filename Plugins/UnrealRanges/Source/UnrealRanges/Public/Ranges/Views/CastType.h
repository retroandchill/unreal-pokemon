// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Utilities/Casts.h"

namespace UE::Ranges {

    template <typename T>
    struct TCastType {

        template <typename R, typename S = ranges::range_value_t<R>>
            requires ranges::input_range<R>
        auto operator()(R &&Range) const {
            if constexpr (UObjectPointer<S> && std::is_base_of_v<UObject, T>) {
                return Range | Map([](S Object) { return CastChecked<T>(Object); });
            } else if constexpr (std::is_base_of_v<FScriptInterface, S> && std::is_base_of_v<UObject, T>) {
                return Range | Map(&CastInterfaceChecked<T>);
            } else if constexpr (UObjectPointer<S> && UnrealInterface<T>) {
                return Range | Map([](S Object) { return TScriptInterface<T>(Object); });
            } else if constexpr (std::is_base_of_v<FScriptInterface, S> && UnrealInterface<T>) {
                return Range | Map([](const FScriptInterface& Interface) { return TScriptInterface<T>(Interface.GetObject()); });
            } else {
                return Range | Map([](S Object) { return static_cast<T>(Object); });
            }
        }
        
    };

    template <typename T>
    constexpr ranges::views::view_closure<TCastType<T>> CastType;
    
}