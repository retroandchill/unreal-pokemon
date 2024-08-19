// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/view/single.hpp>

namespace UE::Ranges {

    /**
     * Create a view from a single element.
     */
    constexpr auto Single = ranges::views::single;
    
}