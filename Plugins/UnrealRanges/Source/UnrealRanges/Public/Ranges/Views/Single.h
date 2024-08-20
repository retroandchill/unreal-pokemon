// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/single.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * Create a view from a single element.
     */
    constexpr auto Single = ranges::views::single;
    
}