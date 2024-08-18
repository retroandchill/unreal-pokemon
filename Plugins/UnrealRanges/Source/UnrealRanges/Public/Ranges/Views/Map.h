// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Functional/Bindings.h"
#include <range/v3/view/transform.hpp>

namespace UE::Ranges {

    struct FMap {

        template <typename... A>
        FORCEINLINE constexpr auto operator()(A&&... Args) const {
            return ranges::views::transform(CreateBinding<A...>(Forward<A>(Args)...));
        }
        
    };
    
    constexpr FMap Map;
    
}
