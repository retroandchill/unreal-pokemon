// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Types.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"

namespace UE::Ranges {

    template <typename F, typename I>
    struct TReduceInvoker {
        constexpr explicit TReduceInvoker(F &&Functor, I &&Identity) : Functor(MoveTemp(Functor)), Identity(Identity) {
        }

        template <typename R>
            requires ranges::input_range<R> || UEContainer<R>
        constexpr auto operator()(R &&Range) {
            using RangeElementType = TRangeCommonReference<R>;
            auto RunningTotal = Identity;
            for (RangeElementType Elem : Range) {
                RunningTotal = std::invoke(Functor, RunningTotal, Elem);
            }
            return RunningTotal;
        }

      private:
        F Functor;
        I Identity;
    };

    struct FReduce {

        template <typename I, typename F, typename... A>
            requires FunctionalType<F>
        constexpr auto operator()(I &&Identity, F &&Functor, A &&...Args) const {
            using BindingType = decltype(CreateBinding<F, A...>(Forward<F>(Functor), Forward<A>(Args)...));
            return TTerminalClosure<TReduceInvoker<BindingType, I>>(TReduceInvoker<BindingType, I>(
                CreateBinding<F, A...>(Forward<F>(Functor), Forward<A>(Args)...), Forward<I>(Identity)));
        }
    };

    /**
     * Performs a reduction on the elements of this view, using the provided identity value and an associative
     * accumulation function, and returns the reduced value.
     */
    constexpr FReduce Reduce;

} // namespace UE::Ranges