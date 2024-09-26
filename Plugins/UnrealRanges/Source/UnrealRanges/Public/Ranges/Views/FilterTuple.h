// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Tuples.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/filter.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    struct FFilterTuple {
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using F = decltype(CreateBinding<A...>(Forward<A>(Args)...));
            return ranges::views::filter([Binding = CreateBinding<A...>(Forward<A>(Args)...)]<typename T>
                    requires CanApply<F, T> (T&& Tuple) {
                return ranges::tuple_apply(Binding, Forward<T>(Tuple));
            });
        }
    };

    /**
     * Equivalent to ranges::views::filter in the STL/range-v3, this version differs in that there are several things
     * you can supply to it in order to create a valid binding.
     */
    constexpr FFilterTuple FilterTuple;

} // namespace UE::Ranges