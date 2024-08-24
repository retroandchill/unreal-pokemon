// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/range/concepts.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {

    /**
     * Object used to handle a closure for a terminal pipe operation with ranges
     */
    template <typename F>
    struct TTerminalClosure {
        /**
         * Default constructor, used for constant closure declarations.
         */
        explicit constexpr TTerminalClosure() = default;

        /**
         * Construct a new instance by moving the target functor into the closure.
         * @param Functor The functor to invoke.
         */
        explicit constexpr TTerminalClosure(F &&Functor) : Functor(MoveTemp(Functor)) {
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
        friend constexpr auto operator|(R &&Range, TTerminalClosure Closure) {
            return Closure.Functor(Forward<R>(Range));
        }

        template <typename R>
            requires UEContainer<R>
        friend constexpr auto operator|(R &Range, TTerminalClosure Closure) {
            return Closure.Functor(Range);
        }

      private:
        F Functor = F();
    };

} // namespace UE::Ranges
