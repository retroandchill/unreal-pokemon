// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * A variadic template structure that inherits from multiple callback-like types
     * and aggregates their operator() functions.
     *
     * TOverloaded allows you to create a single callable object that can handle
     * various types and forms of invocations by inheriting from multiple types
     * each of which implements an operator() function.
     *
     * Template Parameters:
     * @tparam T A variadic template parameter pack representing the types from which
     *      TOverloaded will inherit. Each of these types must implement an
     *      operator() function.
     */
    template <typename... T>
    struct TOverloaded : T... {
        using T::operator()...;
    };
} // namespace UE::Ranges