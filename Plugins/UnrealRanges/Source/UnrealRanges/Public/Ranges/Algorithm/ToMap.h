// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/Types.h"
#include "Ranges/TerminalClosure.h"

THIRD_PARTY_INCLUDES_START
#include <range/v3/range/concepts.hpp>
THIRD_PARTY_INCLUDES_END

namespace UE::Ranges {
    template <typename K, typename V>
    struct TToMapInvoker {
        /**
         * Construct a new invoker from the provided functor.
         * @param KeyFunctor The functor to call to get the keys
         * @param ValueFunctor The functor to call to get the values
         */
        constexpr TToMapInvoker(K &&KeyFunctor, V &&ValueFunctor)
            : KeyFunctor(MoveTemp(KeyFunctor)), ValueFunctor(MoveTemp(ValueFunctor)) {
        }

        /**
         * Perform the for each loop on the specified range.
         * @tparam R The type of range that was passed through
         * @param Range The range to iterate over.
         */
        template <typename R>
            requires ranges::input_range<R> || UEContainer<R>
        auto operator()(R &&Range) const {
            using RangeType = TRangeCommonReference<R>;
            using KeyType = std::remove_cvref_t<decltype(std::invoke(KeyFunctor, std::declval<RangeType>()))>;
            using ValueType = std::remove_cvref_t<decltype(std::invoke(ValueFunctor, std::declval<RangeType>()))>;

            TMap<KeyType, ValueType> Result;
            for (RangeType Data : Range) {
                auto Key = std::invoke(KeyFunctor, Data);
                check(!Result.Contains(Key))
                Result.Emplace(MoveTemp(Key), std::invoke(ValueFunctor, Data));
            }

            return Result;
        }

      private:
        K KeyFunctor;
        V ValueFunctor;
    };

    template <typename K>
    struct TToMapValueBinding {

        /**
         * Construct a new invoker from the provided functor.
         * @param Functor The functor to call back on.
         */
        constexpr explicit TToMapValueBinding(K &&Functor) : Functor(MoveTemp(Functor)) {
        }

        /**
         * Functional invocation that is used to perform the
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename... A>
        constexpr auto operator()(A &&...Args) {
            using BindingType = decltype(CreateBinding<A...>(Forward<A>(Args)...));
            return TTerminalClosure<TToMapInvoker<K, BindingType>>(
                TToMapInvoker<K, BindingType>(MoveTemp(Functor), CreateBinding<A...>(Forward<A>(Args)...)));
        }

      private:
        K Functor;
    };

    struct FToMapKeyBinding {

        /**
         * Functional invocation that is used to perform the
         * @tparam A The type(s) of any additional arguments to bind to the callback.
         * @param Args The types of the arguments in the binding.
         * @return The bound closure.
         */
        template <typename... A>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = decltype(CreateBinding<A...>(Forward<A>(Args)...));
            return TToMapValueBinding<BindingType>(CreateBinding<A...>(Forward<A>(Args)...));
        }
    };

    /**
     * Terminal invoker for ending a range pipe by performing a for each loop on the closure.
     */
    inline constexpr FToMapKeyBinding ToMap;

} // namespace UE::Ranges
