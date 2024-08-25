// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OrElseGet.h"

namespace UE::Optionals {

    template <typename T>
    struct TOrElseInvoker {

        explicit constexpr TOrElseInvoker(const T& Value) : Value(Value) {}
        explicit constexpr TOrElseInvoker(T&& Value) requires (!std::is_lvalue_reference_v<T>) : Value(MoveTemp(Value)) {}

        /**
         * Obtain the underlying element of an optional, invoking the functor if nothing is present.
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The obtained value
         */
        template <typename O>
            requires UEOptional<O>
        constexpr auto operator()(O&& Optional) {
            using ResultType = TOptionalElementType<O>;
            return Optional.IsSet() ? *Optional : ResultType(Forward<T>(Value));
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
        constexpr auto operator()(T&& Value) const {
            return TOptionalClosure<TOrElseInvoker<T>>(TOrElseInvoker<T>(Forward<T>(Value)));
        }
        
    };

    /**
     * Get the value contained within the optional, or alternatively the supplied value.
     */
    constexpr FOrElse OrElse;
    
}
