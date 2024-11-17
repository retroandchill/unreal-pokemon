// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "OptionalClosure.h"
#include "Ranges/Functional/Bindings.h"
#include "Ranges/RangeConcepts.h"
#include "Traits.h"

namespace UE::Optionals {

    /**
     * Optional closure invocation used to invoke on an optional to get an alternate value
     * @tparam F The functor to invoke for alternate invocations
     */
    template <typename F>
    struct TOrElseGetInvoker {

        template <typename T>
            requires std::constructible_from<F, T> && (!std::same_as<std::remove_cvref_t<T>, TOrElseGetInvoker>)
        explicit constexpr TOrElseGetInvoker(T &&Functor) : Functor(std::forward<T>(Functor)) {
        }

        /**
         * Obtain the underlying element of an optional, invoking the functor if nothing is present.
         * @tparam O The type of the passed optional
         * @param Optional The passed optional
         * @return The obtained value
         */
        template <typename O>
            requires UEOptional<O>
        constexpr decltype(auto) operator()(O &&Optional) const {
            using FunctorResult = std::invoke_result_t<F>;
            using ContainedType = TContainedOptionalType<O>;
            constexpr bool bReturnReference = std::is_lvalue_reference_v<FunctorResult> &&
                                              std::is_lvalue_reference_v<ContainedType> &&
                                              std::convertible_to<FunctorResult, ContainedType>;
            using ResultType = std::conditional_t<bReturnReference, ContainedType, TOptionalElementType<O>>;
            if constexpr (std::is_rvalue_reference_v<O>) {
                return Optional.IsSet() ? ResultType(MoveTempIfPossible(Optional.GetValue())) : ResultType(Functor());
            } else {
                return Optional.IsSet() ? ResultType(Optional.GetValue()) : ResultType(Functor());
            }
        }

      private:
        F Functor;
    };

    /**
     * Optional closure creator to get an alternate value out of an optional if necessary.
     */
    struct FOrElseGet {

        /**
         * Create the optional closure to operate with.
         * @tparam A The arguments to supply to the functor.
         * @param Args The arguments to supply to the functor.
         * @return The bound functional closure.
         */
        template <typename... A>
            requires Ranges::CanCreateBinding<A...>
        constexpr auto operator()(A &&...Args) const {
            using BindingType = std::decay_t<decltype(Ranges::CreateBinding<A...>(std::forward<A>(Args)...))>;
            return TOptionalClosure<TOrElseGetInvoker<BindingType>>(
                TOrElseGetInvoker<BindingType>(Ranges::CreateBinding<A...>(std::forward<A>(Args)...)));
        }
    };

    /**
     * Get the value of an optional, getting out the value as supplied by the contained functor if the value is not
     * present.
     */
    constexpr FOrElseGet OrElseGet;
} // namespace UE::Optionals
