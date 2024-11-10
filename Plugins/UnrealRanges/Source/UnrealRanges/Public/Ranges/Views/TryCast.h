// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "FilterValid.h"
#include "Map.h"
#include "Ranges/Concepts/UObjectPointer.h"
#include "Ranges/Utilities/Casts.h"
#include "Ranges/Utilities/Unreachable.h"

namespace UE::Ranges {
    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    struct TTryCast {

        template <typename R, typename S = ranges::range_value_t<R>>
            requires ranges::input_range<R>
        auto operator()(R &&Range) const {
            if constexpr (UObjectPointer<S> && std::is_base_of_v<UObject, T>) {
                return Range | Map([](S Object) { return Cast<T>(Object); }) | FilterValid;
            } else if constexpr (std::is_base_of_v<FScriptInterface, S> && std::is_base_of_v<UObject, T>) {
                return Range | Map(&CastInterface<T>) | FilterValid;
            } else if constexpr (UObjectPointer<S> && UnrealInterface<T>) {
                return Range | Filter([](S Object) { return Object->template Implements<typename T::UClassType>(); }) |
                       Map([](S Object) { return TScriptInterface<T>(Object); });
            } else if constexpr (std::is_base_of_v<FScriptInterface, S> && UnrealInterface<T>) {
                return Range | Filter([](const FScriptInterface &Interface) {
                           return Interface.GetObject()->Implements<typename T::UClassType>();
                       }) |
                       Map([](const FScriptInterface &Interface) {
                           return TScriptInterface<T>(Interface.GetObject());
                       });
            }

            Unreachable();
        }
    };

    /**
     * Cast the type to another type. This will automatically handle the conversion between script interfaces and
     * UObjects without the need to explicitly specify that. Any casts that fail are automatically filtered out from
     * the resultant view.
     *
     * @tparam T The type to cast to.
     */
    template <typename T>
        requires std::derived_from<T, UObject> || UnrealInterface<T>
    inline constexpr ranges::views::view_closure<TTryCast<T>> TryCast;
} // namespace UE::Ranges
