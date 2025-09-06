/**
 * @file OrElseThrow.h
 * @brief Get the value contained within an optional, otherwise throw a std::bad_optional_access.
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

namespace Retro::Optionals
{

    struct FOrElseThrowInvoker
    {
        template <OptionalType O>
        constexpr decltype(auto) operator()(O &&Optional) const
        {
            return (*this)(std::forward<O>(Optional), [] { return std::bad_optional_access{}; });
        }

        /**
         * Invokes the functor with the provided arguments and throws the result if the optional parameter does not have
         * a value. If the optional parameter does have a value, retrieves and returns it.
         *
         * @param optional The optional object to check for a value.
         * @param functor The functor to invoke in case the optional does not contain a value.
         * @return The value contained in the optional object if it exists.
         * @throw The result of invoking the functor with the provided arguments if the optional does not have a value.
         */
        template <OptionalType O, typename F>
            requires std::invocable<F> && std::derived_from<std::invoke_result_t<F>, std::exception>
        constexpr decltype(auto) operator()(O &&optional, F &&functor) const
        {
            if (HasValue(std::forward<O>(optional)))
            {
                return Get<O>(std::forward<O>(optional));
            }

            throw std::invoke(std::forward<F>(functor));
        }
    };

    RETROLIB_EXPORT constexpr auto OrElseThrow = ExtensionMethod<FOrElseThrowInvoker{}>;

} // namespace Retro::Optionals