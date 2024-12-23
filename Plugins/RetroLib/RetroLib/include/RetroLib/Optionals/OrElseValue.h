/**
 * @file OrElseValue.h
 * @brief Get the contents of an optional
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Functional/CreateBinding.h"
#include "RetroLib/Functional/FunctionalClosure.h"
#include "RetroLib/Optionals/Optional.h"
#include "RetroLib/Optionals/OptionalOperations.h"
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {
    struct OrElseValueInvoker {

        /**
         * Functional call operator that returns the value of the provided optional object
         * if it contains a value; otherwise, returns the provided fallback value.
         *
         * @param optional The optional object to be checked for a valid value.
         * @param value The fallback value to be returned if the optional object does not contain a value.
         * @return The value contained in the optional object if it has one; otherwise, the provided fallback value.
         */
        template <OptionalType O, typename T>
            requires std::convertible_to<CommonReference<O>, T> &&
                     (!std::is_lvalue_reference_v<CommonReference<O>> ||
                      !SpecializationOf<std::decay_t<T>, std::reference_wrapper>)
        constexpr decltype(auto) operator()(O &&optional, T &&value) const {
            if (has_value(optional)) {
                return static_cast<T &&>(get(std::forward<O>(optional)));
            }

            return std::forward<T>(value);
        }

        /**
         * Overloaded function call operator that retrieves a value from an optional-like
         * object if it contains a value; otherwise, returns the provided reference wrapper.
         *
         * @tparam O The type of the optional-like object.
         * @tparam T The type of the referenced value.
         * @param optional The optional-like object to check for a contained value.
         * @param value A reference wrapper containing the fallback value.
         * @return A reference to the contained value in the optional-like object if it exists;
         *         otherwise, the fallback value from the provided reference wrapper.
         */
        template <OptionalType O, typename T>
            requires std::convertible_to<CommonReference<O>, T> && std::is_lvalue_reference_v<CommonReference<O>>
        constexpr T &operator()(O &&optional, const std::reference_wrapper<T> &value) const {
            if (has_value(optional)) {
                return get(std::forward<O>(optional));
            }

            return value;
        }
    };

    /**
     * Extension method that provides functionality to return the value of a given optional object
     * if it contains a valid value; otherwise, it returns the specified fallback value.
     *
     * Intended for use where inline handling of optional values with a default fallback is required.
     */
    RETROLIB_EXPORT constexpr auto or_else_value = extension_method<OrElseValueInvoker{}>;
} // namespace retro::optionals
