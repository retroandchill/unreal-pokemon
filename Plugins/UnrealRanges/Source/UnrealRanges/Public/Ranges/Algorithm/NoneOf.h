// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Algo/NoneOf.h"
#include "range/v3/algorithm/none_of.hpp"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/TerminalClosure.h"

namespace UE::Ranges {

    /**
     * Terminal invoker for performing a none of operation
     * @tparam F The type of the functor to call as part of the for each loop.
     */
    template <typename F>
    struct TNoneOfInvoker {
        /**
         * Construct a new invoker from the provided functor.
         * @param Functor The functor to call back on.
         */
        constexpr explicit TNoneOfInvoker(F &&Functor) : Functor(std::move(Functor)) {
        }

        /**
         * Perform the for each loop on the specified range.
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R>
            requires ranges::input_range<R>
        bool operator()(R &&Range) const {
            return ranges::none_of(std::forward<R>(Range), Functor);
        }

        template <typename R>
            requires UEContainer<R>
        bool operator()(R &Range) const {
            return Algo::NoneOf(Range, Functor);
        }

      private:
        F Functor;
    };

    /**
     * Closure creation struct for handling none of operations.
     */
    struct FNoneOf {

        /**
         * Functional invocation that is used to perform the
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(CreateBinding<A...>(std::forward<A>(Args)...));
            return TTerminalClosure<TNoneOfInvoker<BindingType>>(
                TNoneOfInvoker<BindingType>(CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Terminal invoker for ending a range pipe by check if none of the elements match the condition
     */
    inline constexpr FNoneOf NoneOf;

} // namespace UE::Ranges