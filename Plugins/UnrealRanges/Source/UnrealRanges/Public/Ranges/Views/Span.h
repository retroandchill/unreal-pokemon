// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/view/span.hpp>

namespace UE::Ranges {

    template <typename T, ranges::detail::span_index_t N = ranges::dynamic_extent>
    using TSpan = ranges::span<T, N>;
    
}
