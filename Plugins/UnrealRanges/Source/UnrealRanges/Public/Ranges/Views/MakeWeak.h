// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "Ranges/Concepts/WeakReferencable.h"
#include "Ranges/RangeConcepts.h"

namespace UE::Ranges {

    struct FMakeWeak {

        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R> && WeakReferenceable<T>
        auto operator()(R &&Range) const {
            return Range | Map([](T &&Ptr) { return TWeakRefType<T>(Forward<T>(Ptr)); });
        }
    };

    /**
     * Convert a weakly referenceable type (mainly a shared pointer or a UObject pointer) into the weak version of
     * that pointer type.
     */
    constexpr ranges::views::view_closure<FMakeWeak> MakeWeak;

} // namespace UE::Ranges