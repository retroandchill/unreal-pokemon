// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ContainerView.h"
#include "Map.h"
#include "Ranges/RangeConcepts.h"
#include <range/v3/view/join.hpp>
#include <range/v3/view/view.hpp>

namespace UE::Ranges {
    struct FJoin {

        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R> && (ranges::input_range<T> || UEContainer<T>)
        auto operator()(R &&Range) const {
            if constexpr (UEContainer<T>) {
                return Range | Map([](T &&Container) { return CreateRange<std::remove_reference_t<T>>(Container); }) |
                       ranges::views::join;
            } else {
                return Range | ranges::views::join;
            }
        }
    };

    /**
     * Flattens a range of ranges down into a single range. Can also automatically convert a UE container into a range
     * and join those without needing to be explicitly done.
     */
    constexpr ranges::views::view_closure<FJoin> Join;
} // namespace UE::Ranges
