﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Tuples.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/transform.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    struct FMapTuple {
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            return ranges::views::transform(
                [Binding = CreateBinding<A...>(std::forward<A>(Args)...)]<typename T>(T &&Tuple) {
                    return ranges::tuple_apply(Binding, std::forward<T>(Tuple));
                });
        }
    };

    /**
     * Equivalent to ranges::views::transform in the STL/range-v3, this version differs in that there are several things
     * you can supply to it in order to create a valid binding.
     */
    constexpr FMapTuple MapTuple;

} // namespace UE::Ranges