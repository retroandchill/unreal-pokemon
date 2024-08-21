// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/cache1.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    constexpr auto CacheLast = ranges::views::cache1;
    
}