// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"

namespace UE::Ranges {

    template <typename T>
    struct TConstructInvoker {

        template <typename R, typename... A>
            requires ranges::input_range<R>
        constexpr auto operator()(R &&Range, A &&...Args) const {
            if constexpr (sizeof...(A) > 0) {
                return Range | ranges::views::transform(ranges::bind_back(
                                   []<typename... B>(B &&...BoundArgs) { return T(Forward<B>(BoundArgs)...); },
                                   Forward<A>(Args)...));
            } else {
                return Range | ranges::views::transform(
                                   []<typename... B>(B &&...BoundArgs) { return T(Forward<B>(BoundArgs)...); });
            }
        }
    };

    template <typename T>
    struct TConstruct {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            if constexpr (sizeof...(A) > 0) {
                return ranges::make_view_closure(ranges::bind_back(TConstructInvoker<T>(), Forward<A>(Args)...));
            } else {
                return ranges::make_view_closure(TConstructInvoker<T>());
            }
        }
    };

    template <typename T>
    constexpr TConstruct<T> Construct;

} // namespace UE::Ranges