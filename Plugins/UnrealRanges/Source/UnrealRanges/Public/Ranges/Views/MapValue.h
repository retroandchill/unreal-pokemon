// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include <range/v3/view/view.hpp>

namespace UE::Ranges {
    struct FMapValue {

        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R> && MapPair<T>
        auto operator()(R &&Range) const {
            if constexpr (std::is_lvalue_reference_v<T>) {
                return Range | Map([](T Pair) -> auto& { return Pair.Value; });
            } else {
                return Range | Map([](T Pair) { return Pair.Value; });
            }
            
        }
    };


    constexpr ranges::views::view_closure<FMapValue> MapValue;
} // namespace UE::Ranges
