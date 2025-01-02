/**
 * @file OrElseValue.h
 * @brief Get the contents of an optional
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
    struct FOrElseValueInvoker {

        /**
         * Functional call operator that returns the value of the provided optional object
         * if it contains a value; otherwise, returns the provided fallback value.
         *
         * @param Optional The optional object to be checked for a valid value.
         * @param Value The fallback value to be returned if the optional object does not contain a value.
         * @return The value contained in the optional object if it has one; otherwise, the provided fallback value.
         */
        template <OptionalType O, typename T>
            requires std::convertible_to<TCommonReference<O>, T> &&
                     (!std::is_lvalue_reference_v<TCommonReference<O>> ||
                      !SpecializationOf<std::decay_t<T>, std::reference_wrapper>)
        constexpr decltype(auto) operator()(O &&Optional, T &&Value) const {
            if (HasValue(Optional)) {
                return static_cast<T &&>(Get(std::forward<O>(Optional)));
            }

            return std::forward<T>(Value);
        }

        /**
         * Overloaded function call operator that retrieves a value from an optional-like
         * object if it contains a value; otherwise, returns the provided reference wrapper.
         *
         * @tparam O The type of the optional-like object.
         * @tparam T The type of the referenced value.
         * @param Optional The optional-like object to check for a contained value.
         * @param Value A reference wrapper containing the fallback value.
         * @return A reference to the contained value in the optional-like object if it exists;
         *         otherwise, the fallback value from the provided reference wrapper.
         */
        template <OptionalType O, typename T>
            requires std::convertible_to<TCommonReference<O>, T> && std::is_lvalue_reference_v<TCommonReference<O>>
        constexpr T &operator()(O &&Optional, const std::reference_wrapper<T> &Value) const {
            if (HasValue(Optional)) {
                return Get(std::forward<O>(Optional));
            }

            return Value;
        }

        template <OptionalType O>
            requires std::is_pointer_v<TValueType<O>>
        constexpr auto operator()(O &&Optional, std::nullptr_t) const {
            if (HasValue(Optional)) {
                return Get(std::forward<O>(Optional));
            }

            return static_cast<TValueType<O>>(nullptr);
        }
    };

    /**
     * Extension method that provides functionality to return the value of a given optional object
     * if it contains a valid value; otherwise, it returns the specified fallback value.
     *
     * Intended for use where inline handling of optional values with a default fallback is required.
     */
    RETROLIB_EXPORT constexpr auto OrElseValue = ExtensionMethod<FOrElseValueInvoker{}>;
} // namespace retro::optionals
