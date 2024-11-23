// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/concat.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * Concatenate a set of multiple ranges into one larger range.
     */
    constexpr auto Concat = ranges::views::concat;

} // namespace UE::Ranges