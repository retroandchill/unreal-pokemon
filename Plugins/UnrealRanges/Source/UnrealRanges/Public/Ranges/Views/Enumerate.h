// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Arrays.h"
#include <range/v3/view/enumerate.hpp>
#include "Ranges/Views/Map.h"

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
            if constexpr (std::is_lvalue_reference_v<T>) {
                return Map([&Container](N Index) {
                    return GetWithIndex<T, N>(Forward<T>(Container), Index);
                });
            } else {
                return Map([Array = Forward<T>(Container)](N Index) {
                    return GetWithIndex<T, N>(Array, Index);
                });
            }
        }
    };

    template <typename N = size_t>
    constexpr auto ReverseEnumerate = TReverseEnumerate<N>();
}
