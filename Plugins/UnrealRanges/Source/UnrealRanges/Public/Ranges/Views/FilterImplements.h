// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/UObjectPointer.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Utilities/Interfaces.h"
#include "Ranges/Utilities/Unreachable.h"
#include "Ranges/Views/Filter.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/view.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    template <typename T>
        requires UnrealInterface<T>
    struct TFilterImplements {

        template <typename R, typename S = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R> && (UObjectPointer<S> || DereferencesToUObject<S> || std::derived_from<FScriptInterface, std::decay_t<S>>)
        constexpr decltype(auto) operator()(R &&Range) const {
            if constexpr (UObjectPointer<S>) {
                return Range | Filter([](const UObject* Object) { return Object->Implements<typename T::UClassType>(); });
            } else if constexpr (std::is_base_of_v<FScriptInterface, std::decay_t<S>>) {
                if constexpr (std::derived_from<TInterfaceType<S>, T>) {
                    return std::forward<R>(Range);
                } else {
                    return Range | Filter([](const FScriptInterface &Interface) { return Interface.GetObject()->Implements<typename T::UClassType>(); });
                }
            } else if constexpr (DereferencesToUObject<T>) {
                return Range | Filter([](const T &Ptr) { return Ptr.Get()->template Implements<typename T::UClassType>(); });
            }

            Unreachable();
        }
    };

    /**
     * Filters that a given UObject type implements a specific interface.
     * 
     * @tparam T The interface to check against
     */
    template <typename T>
        requires UnrealInterface<T>
    inline constexpr ranges::views::view_closure<TFilterImplements<T>> FilterImplements;

} // namespace UE::Ranges