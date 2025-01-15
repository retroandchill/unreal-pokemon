/**
 * @file Filter.h
 * @brief Closure for filtering an optional value
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    RETROLIB_EXPORT template <OptionalType O>
    struct TConstructEmpty {
        constexpr O operator()() const requires std::is_default_constructible_v<O> {
            return O();
        }
    };

    template <ExpectedType O>
    struct TConstructEmpty<O> {
        constexpr O operator()() const requires std::is_default_constructible_v<TErrorType<O>> {
            return PassError<O>(TErrorType<O>());
        }

        template <typename E>
            requires std::convertible_to<E, TErrorType<O>>
        constexpr O operator()(E&& Error) const {
            return PassError<O>(std::forward<E>(Error));
        }

        template <typename F>
            requires std::invocable<F> && std::convertible_to<std::invoke_result_t<F>, TErrorType<O>>
        constexpr O operator()(F&& Supplier) const {
            return PassError<O>(std::invoke(std::forward<F>(Supplier)));
        }
    };

    RETROLIB_EXPORT template <OptionalType O>
    constexpr TConstructEmpty<O> ConstructEmpty;

    template <typename O, typename... A>
    concept Filterable = requires(O&& Optional, A&&... Args) {
        { Retro::Optionals::ConstructEmpty<std::remove_cvref_t<O>>(std::forward<A>(Args)...) } -> std::same_as<std::remove_cvref_t<O>>;
    };

    struct FFilterInvoker {
        /**
         * @brief Applies the given functor to the value within an optional-like object and returns the optional based
         * on the functor's result.
         *
         * This function checks if the optional-like object contains a value and applies a functor to the value to
         * decide whether to return the same optional-like object or an empty one. If the functor returns true, the
         * optional-like object is returned as is, otherwise an empty one is returned.
         *
         * @tparam O The type of the optional-like object, can be an lvalue or rvalue reference.
         * @tparam F The type of the functor to be applied on the contained value.
         *
         * @param Optional An optional-like object whose value is evaluated and possibly passed to the functor.
         * @param Functor A callable object which takes the value from the optional and returns a boolean.
         *
         * @return If the optional contains a value and the functor returns true, the optional-like object is returned
         * with its value preserved. Otherwise, an empty optional-like object of the same type is returned.
         */
        template <OptionalType O, typename F, typename... A>
            requires std::is_invocable_r_v<bool, F, TCommonReference<O>> && Filterable<O, A...>
        constexpr auto operator()(O &&Optional, F &&Functor, A&&... Args) const {
            if constexpr (std::is_lvalue_reference_v<O>) {
                using FilteredType = decltype(MakeOptionalReference(std::forward<O>(Optional)));
                static_assert(Filterable<FilteredType>);
                return HasValue(std::forward<O>(Optional)) &&
                               std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional)))
                           ? MakeOptionalReference(std::forward<O>(Optional))
                           : ConstructEmpty<FilteredType>(std::forward<A>(Args)...);
            } else {
                return HasValue(std::forward<O>(Optional)) &&
                               std::invoke(std::forward<F>(Functor), Get<O>(std::forward<O>(Optional)))
                           ? O(std::forward<O>(Optional))
                           : ConstructEmpty<O>(std::forward<A>(Args)...);
            }
        }
    };

    RETROLIB_EXPORT constexpr auto Filter = ExtensionMethod<FFilterInvoker{}>;

} // namespace Retro::Optionals