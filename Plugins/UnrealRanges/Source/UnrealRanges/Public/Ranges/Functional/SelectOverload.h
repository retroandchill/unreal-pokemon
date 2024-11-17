// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * @class TSelectOverload
     *
     * @brief A utility class designed to help with selecting function overloads.
     *
     * This class template provides a mechanism to select a specific function overload
     * in cases where multiple overloads are possible. By creating an instance of this
     * class template, a function pointer to the desired overload can be specified.
     *
     * @tparam A The argument types of the function overload.
     */
    template <typename... A>
    struct TSelectOverload {
        template <typename R>
        constexpr auto operator()(R(*Functor)(A...)) const {
            return Functor;
        }

        template <typename R, typename C>
        constexpr auto operator()(R(C::*Functor)(A...)) const {
            return Functor;
        }
    };

    /**
     * @brief A constant instance of the TSelectOverload utility class.
     *
     * This instance is used to select a specific function overload where multiple
     * overloads are possible. By creating this constant, it provides a simple
     * way to reference TSelectOverload without explicitly instantiating it.
     *
     * @tparam A The argument types of the function overload.
     */
    template <typename... A>
    constexpr TSelectOverload<A...> SelectOverload;
}
