// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Arrays.h"
#include "Ranges/Utilities/Arrays.h"
#include "Ranges/Views/Map.h"

#include <range/v3/view/enumerate.hpp>

namespace UE::Ranges {

    /**
     * Add the index to the enumeration totals.
     */
    constexpr auto Enumerate = ranges::views::enumerate;

    template <typename N = size_t>
    struct TReverseEnumerate {

        template <typename T>
            requires Indexable<T, size_t>
        constexpr auto operator()(T&& Container) const {
            return Map([&Container](N Index) {
                return GetWithIndex<T, N>(Forward<T>(Container), Index);
            });
        }
    };

    template <typename N = size_t>
    constexpr TReverseEnumerate<N> ReverseEnumerate;
}
