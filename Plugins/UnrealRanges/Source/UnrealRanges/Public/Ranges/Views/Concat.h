// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/view/concat.hpp>

namespace UE::Ranges {

    /**
     * Concatenate a set of multiple ranges into one larger range.
     */
    constexpr auto Concat = ranges::views::concat;
    
}