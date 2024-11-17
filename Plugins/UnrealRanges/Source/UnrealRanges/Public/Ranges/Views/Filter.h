// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/Bindings.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/filter.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    struct FFilter {
        template <typename... A>
            requires CanCreateBinding<A...>
        constexpr auto operator()(A &&...Args) const {
            return ranges::views::filter(CreateBinding<A...>(std::forward<A>(Args)...));
        }
    };

    /**
     * Equivalent to ranges::views::filter in the STL/range-v3, this version differs in that there are several things
     * you can supply to it in order to create a valid binding.
     */
    constexpr FFilter Filter;

} // namespace UE::Ranges
