// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/Bindings.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/transform.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    struct FMap {
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            return ranges::views::transform(CreateBinding<A...>(Forward<A>(Args)...));
        }
    };

    /**
     * Equivalent to ranges::views::transform in the STL/range-v3, this version differs in that there are several things
     * you can supply to it in order to create a valid binding.
     */
    constexpr FMap Map;

} // namespace UE::Ranges
