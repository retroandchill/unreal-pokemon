// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/Concepts/WeakReferencable.h"

namespace UE::Ranges {

    struct FMakeStrong {

        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R> && WeakReference<T>
        auto operator()(R &&Range) const {
            return Range | Map([](T &&Ptr) { return ToStrongRef<T>(Forward<T>(Ptr)); });
        }
        
    };

    /**
     * Extract a weakly referenceable type (mainly a shared pointer or a UObject pointer) from the weak version of
     * that pointer type.
     */
    constexpr ranges::views::view_closure<FMakeStrong> MakeStrong;

}