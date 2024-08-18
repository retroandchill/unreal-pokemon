// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/range/concepts.hpp>

namespace UE::Ranges {

/**
 * Object used to handle a closure for a terminal pipe operation with ranges
 */
template <typename F>
class TTerminalClosure {
public:
    /**
     * Default constructor, used for constant closure declarations.
     */
    explicit constexpr TTerminalClosure() = default;
    
    /**
     * Construct a new instance by moving the target functor into the closure.
     * @param Functor The functor to invoke.
     */
    explicit constexpr TTerminalClosure(F&& Functor) : Functor(MoveTemp(Functor)) {
    }

    /**
     * Operate on the functor for the closure performing the operation on the target range.
     * @tparam R The type of range that was passed in.
     * @param Range The range to operate on
     * @param Closure The closure being passed in
     * @return The result of the closure operation (can be void depending on the closure)
     */
    template <typename R>
        requires ranges::input_range<R> && std::is_invocable_v<F, R>
    FORCEINLINE friend constexpr auto operator|(R&& Range, TTerminalClosure Closure) {
        return Closure.Functor(Forward<R>(Range));
    }

private:
    F Functor = F();
};

}
