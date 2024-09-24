// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/view/enumerate.hpp>

namespace UE::Ranges {

    /**
     * Add the index to the enumeration totals.
     */
    constexpr auto Enumerate = ranges::views::enumerate;
}
