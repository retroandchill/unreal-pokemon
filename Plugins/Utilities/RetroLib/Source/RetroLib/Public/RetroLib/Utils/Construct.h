// "Unreal Pokémon" created by Retro & Chill.

#pragma once
#if !RETROLIB_WITH_MODULES
#include <concepts>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro
{

    /**
     * @class TConstruct
     * @brief The TConstruct class is used as a functional object to allow for the construction of a class with the
     * provided parameters.
     */
    RETROLIB_EXPORT template <typename T>
    struct TConstruct
    {
        /**
         * Constructs an object of type T with the provided arguments.
         *
         * @param Args Variadic template parameters for the constructor of type T.
         * @return An instance of type T constructed with the provided arguments.
         */
        template <typename... A>
            requires std::constructible_from<T, A...>
        constexpr T operator()(A &&...Args) const
        {
            return T(std::forward<A>(Args)...);
        }
    };

    /**
     * Functional object to allow for the construction of a class with the provided parameters.
     */
    template <typename T>
    constexpr TConstruct<T> Construct;
} // namespace Retro
