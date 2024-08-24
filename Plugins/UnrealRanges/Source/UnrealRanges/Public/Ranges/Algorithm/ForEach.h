// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "range/v3/algorithm/for_each.hpp"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Algo/ForEach.h"

namespace UE::Ranges {

    /**
     * Terminal invoker for performing the for each termination.
     * @tparam F The type of the functor to call as part of the for each loop.
     */
    template <typename F>
    struct TForEachInvoker {
        /**
         * Construct a new invoker from the provided functor.
         * @param Functor The functor to call back on.
         */
        constexpr explicit TForEachInvoker(F&& Functor) : Functor(MoveTemp(Functor)) {
        }

        /**
         * Perform the for each loop on the specified range.
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R>
            requires ranges::input_range<R>
        void operator()(R&& Range) const {
            ranges::for_each(Forward<R>(Range), Functor);
        }

        template <typename R>
            requires UEContainer<R>
        void operator()(R &Range) const {
            Algo::ForEach(Range, Functor);
        }

    private:
        F Functor;
    };

    /**
     * Closure creation struct for handling for each operations.
     */
    struct FForEach {

        /**
         * Functional invocation that is used to perform the 
         * @tparam F The type of the functor.
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Functor The functor to bind to the for each loop.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename F, typename... A>
            requires FunctionalType<F>
        constexpr auto operator()(F &&Functor, A&&... Args) const {
            using BindingType = decltype(CreateBinding<F, A...>(Forward<F>(Functor), Forward<A>(Args)...));
            return TTerminalClosure<TForEachInvoker<BindingType>>(TForEachInvoker<BindingType>(
                CreateBinding<F, A...>(Forward<F>(Functor), Forward<A>(Args)...)));
        } 
        
    };

    /**
     * Terminal invoker for ending a range pipe by performing a for each loop on the closure.
     */
    inline constexpr FForEach ForEach;
    
}