// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Tuples.h"

namespace UE::Ranges {

    class FTryInvoke {
      public:
        template <typename F, typename... A>
        auto operator()(F &&Functor, A &&...Args) const {
            return ranges::invoke(std::forward<F>(Functor), std::forward<A>(Args)...);
        }

        template <typename F, typename T>
            requires CanApply<F, T>
        auto operator()(F &&Functor, T &&Tuple) const {
            return ranges::tuple_apply(std::forward<F>(Functor), std::forward<T>(Tuple));
        }
    };

    constexpr auto TryInvoke = FTryInvoke();

} // namespace UE::Ranges