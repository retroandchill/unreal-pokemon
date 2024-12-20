﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Algo/ForEach.h"
#include "range/v3/algorithm/for_each.hpp"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"

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
        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::decay_t<T>, TForEachInvoker>)
        constexpr explicit TForEachInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        /**
         * Perform the for each loop on the specified range.
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R, typename T = ranges::range_common_reference_t<R>>
            requires ranges::input_range<R>
        void operator()(R &&Range) const {
            ranges::for_each(std::forward<R>(Range), Functor);
        }

        template <typename R>
            requires UEContainer<R>
        void operator()(R &Range) const {
            for (auto &Elem : Range) {
                std::invoke(Functor, Elem);
            }
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
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename... A>
            requires CanCreateBinding<A...>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(CreateBinding<A...>(std::forward<A>(Args)...));
            return TTerminalClosure<TForEachInvoker<BindingType>>(
                TForEachInvoker<BindingType>(CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Terminal invoker for ending a range pipe by performing a for each loop on the closure.
     */
    inline constexpr FForEach ForEach;

} // namespace UE::Ranges