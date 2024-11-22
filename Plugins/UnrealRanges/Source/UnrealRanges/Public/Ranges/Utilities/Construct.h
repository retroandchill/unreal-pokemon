// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    
    /**
     * @class TConstruct
     * @brief The TConstruct class is used as a functional object to allow for the construction of a class with the
     * provided parameters.
     */
    template <typename T>
    struct TConstruct {
        template <typename... A>
            requires std::constructible_from<T, A...>
        constexpr T operator()(A&&... Args) const {
            return T(std::forward<A>(Args)...);
        }
    };

    /**
     * Functional object to allow for the construction of a class with the provided parameters.
     */
    template <typename T>
    constexpr TConstruct<T> Construct;
}
