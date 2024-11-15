// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Map.h"
#include "Ranges/Functional/Bindings.h"

namespace UE::Optionals {

    template <typename T>
    struct TConstructInvoker {

        template <typename O, typename... A>
            requires UEOptional<O>
        constexpr auto operator()(O &&Optional, A &&...Args) const {
            return Optional.IsSet() ? TOptional<T>(T(Ranges::ForwardLike<O>(*Optional), std::forward<A>(Args)...))
                                    : TOptional<T>();
        }
    };

    template <typename T>
    struct TConstruct {

        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            if constexpr (sizeof...(A) > 0) {
                using BackBound = decltype(ranges::bind_back(TConstructInvoker<T>(), std::forward<A>(Args)...));
                return TOptionalClosure<BackBound>(ranges::bind_back(TConstructInvoker<T>(), std::forward<A>(Args)...));
            } else {
                return TOptionalClosure<TConstructInvoker<T>>(TConstructInvoker<T>());
            }
        }
    };

    /**
     * Map the optional to a new value if present, otherwise return an empty optional.
     */
    template <typename T>
    constexpr TConstruct<T> Construct;
} // namespace UE::Optionals
