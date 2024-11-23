// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Types.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/Functional/TryInvoke.h"
#include "Ranges/RangeConcepts.h"
#include "Ranges/TerminalClosure.h"

namespace UE::Ranges {

    /**
     * Terminal invoker for grouping a range into a map of arrays
     * @tparam F The type of the functor to call as part of the for each loop.
     */
    template <typename F>
    struct TGroupingByInvoker {
        /**
         * Construct a new invoker from the provided functor.
         * @param Functor The functor to call back on.
         */
        constexpr explicit TGroupingByInvoker(F &&Functor) : Functor(std::move(Functor)) {
        }

        /**
         * Perform the grouping operation on the range
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R>
            requires ranges::input_range<R> || UEContainer<R>
        auto operator()(R &&Range) const {
            using RangeType = TRangeCommonReference<R>;
            using KeyType = std::remove_cvref_t<decltype(TryInvoke(Functor, std::declval<RangeType>()))>;
            using ValueType = TRangeValue<R>;

            TMap<KeyType, TArray<ValueType>> Result;
            for (RangeType &&Elem : Range) {
                auto &Value = Result.FindOrAdd(TryInvoke(Functor, std::forward<RangeType>(Elem)));
                Value.Emplace(std::forward<RangeType>(Elem));
            }

            return Result;
        }

      private:
        F Functor;
    };

    /**
     * Closure creation struct for handling grouping into a map of lists.
     */
    struct FGroupingBy {

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
            return TTerminalClosure<TGroupingByInvoker<BindingType>>(
                TGroupingByInvoker<BindingType>(CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Terminal invoker for ending a range pipe by placing the elements into a map of arrays.
     */
    // ReSharper disable once IdentifierTypo
    inline constexpr FGroupingBy GroupingBy;

} // namespace UE::Ranges