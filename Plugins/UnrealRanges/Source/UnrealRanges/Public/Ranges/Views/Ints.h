// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/view/iota.hpp>

namespace UE::Ranges {

    /**
     * Create a view that wraps around the integer operation
     */
    constexpr auto Ints = ranges::views::ints;

} // namespace UE::Ranges
