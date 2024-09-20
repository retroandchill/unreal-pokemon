// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Algo/AnyOf.h"
#include "range/v3/algorithm/any_of.hpp"
#include "range/v3/algorithm/for_each.hpp"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/TerminalClosure.h"

namespace UE::Ranges {

    /**
     * Terminal invoker for performing an all of operation
     * @tparam F The type of the functor to call as part of the for each loop.
     */
    template <typename F>
    struct TAllOfInvoker {
        /**
         * Construct a new invoker from the provided functor.
         * @param Functor The functor to call back on.
         */
        constexpr explicit TAllOfInvoker(F &&Functor) : Functor(MoveTemp(Functor)) {
        }

        /**
         * Perform the for each loop on the specified range.
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R>
            requires ranges::input_range<R>
        bool operator()(R &&Range) const {
            return ranges::any_of(Forward<R>(Range), Functor);
        }

        template <typename R>
            requires UEContainer<R>
        bool operator()(R &Range) const {
            return Algo::AnyOf(Range, Functor);
        }

      private:
        F Functor;
    };

    /**
     * Closure creation struct for handling any of operations.
     */
    struct FAllOf {

        /**
         * Functional invocation that is used to perform the
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(CreateBinding<A...>(Forward<A>(Args)...));
            return TTerminalClosure<TAllOfInvoker<BindingType>>(
                TAllOfInvoker<BindingType>(CreateBinding<A...>(Forward<A>(Args)...)));
        }
    };

    /**
     * Terminal invoker for ending a range pipe by check if any elements match the condition
     */
    inline constexpr FAllOf AllOf;

} // namespace UE::Ranges