// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Optional/OptionalClosure.h"
#include "Ranges/Views/Filter.h"
#include "Ranges/Concepts/UObjectPointer.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/view.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    struct FFilterValid {

        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R>
        constexpr auto operator()(R &&Range) const {
            if constexpr (UObjectPointer<T>) {
                return Range | Filter(&IsValid);
            } else if constexpr (std::is_base_of_v<FScriptInterface, std::remove_cvref_t<T>>) {
                return Range | Filter([](const FScriptInterface &Interface) { return IsValid(Interface.GetObject()); });
            } else if constexpr (Optionals::UEOptional<T>) {
                return Range | Filter([](const T &Optional) { return Optional.IsSet(); });
            } else if constexpr (DereferencesToUObject<T>) {
                return Range | Filter([](const T &Ptr) { return IsValid(Ptr.Get()); });
            } else {
                return Range | Filter([](const T &Ptr) { return Ptr != nullptr; });
            }
        }
    };

    /**
     * Performs a filter on the data filtering out any data considered invalid. What is considered invalid varies based
     * on the types. For a UObject/Interface it uses the IsValid() function, TOptional objects must not be empty, and
     * all other types will perform an equality check against nullptr.
     */
    constexpr ranges::views::view_closure<FFilterValid> FilterValid;

} // namespace UE::Ranges