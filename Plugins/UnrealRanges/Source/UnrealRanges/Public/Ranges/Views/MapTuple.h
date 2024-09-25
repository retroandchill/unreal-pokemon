// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Views/Map.h"

namespace UE::Ranges {

    struct FMapTuple {
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            return Map([]<typename T>(T&& Tuple) {
                
            }, Forward<A>(Args)...);
        }
    };

    /**
     * Equivalent to ranges::views::transform in the STL/range-v3, this version differs in that there are several things
     * you can supply to it in order to create a valid binding.
     */
    constexpr FMapTuple MapTuple;

} // namespace UE::Ranges
