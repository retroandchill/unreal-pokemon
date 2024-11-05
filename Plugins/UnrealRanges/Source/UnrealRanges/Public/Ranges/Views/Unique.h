// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/unique.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * @brief A constant representing the unique view in the ranges library.
     *
     * The Unique view adapter returns a view that contains only the first
     * occurrence of each element from a range, effectively removing consecutive
     * duplicate elements.
     *
     * Usage of this constant allows integrating the unique view functionality
     * with various range-based operations.
     */
    inline constexpr auto Unique = ranges::views::unique;
}
