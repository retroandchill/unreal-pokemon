// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/view/any_view.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    template <typename T, ranges::category C = ranges::category::input>
    using TAnyView = ranges::any_view<T, C>;
    
}