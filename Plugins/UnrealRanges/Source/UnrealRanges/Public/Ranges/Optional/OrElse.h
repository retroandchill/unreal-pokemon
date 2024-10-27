﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OrElseGet.h"

namespace UE::Optionals {

    template <typename T>
    struct TOrElseInvoker {

        explicit constexpr TOrElseInvoker(const T &Value) : Value(Value) {
        }
        explicit constexpr TOrElseInvoker(T &&Value)
            requires(!std::is_lvalue_reference_v<T>)
            : Value(std::move(Value)) {
        }

        /**
         * Obtain the underlying element of an optional, invoking the functor if nothing is present.
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The obtained value
         */
        template <typename O, typename S = decltype(std::declval<O>().Get(std::declval<T>()))>
            requires UEOptional<O>
        constexpr S operator()(O &&Optional) {
            return Optional.Get(std::forward<T>(Value));
        }

      private:
        T Value;
    };

    struct FOrElse {

        /**
         * Create the optional closure to operate with.
         * @tparam T The type of the passed value
         * @param Value The value to get as the alternative
         * @return The bound functional closure.
         */
        template <typename T>
        constexpr auto operator()(T &&Value) const {
            return TOptionalClosure<TOrElseInvoker<T>>(TOrElseInvoker<T>(std::forward<T>(Value)));
        }
    };

    /**
     * Get the value contained within the optional, or alternatively the supplied value.
     */
    constexpr FOrElse OrElse;

} // namespace UE::Optionals
