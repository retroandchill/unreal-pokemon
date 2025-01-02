/**
 * @file To.h
 * @brief Helper for converting between various optional types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Functional/ExtensionMethods.h"
#include "RetroLib/FunctionTraits.h"
#include "RetroLib/Optionals/OptionalOperations.h"

#if !RETROLIB_WITH_MODULES
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Optionals {

    /**
     * @brief Converts an optional-like object to an instance of type T.
     *
     * @tparam T The target type to convert to.
     * @tparam F The type of the optional-like object being passed.
     * @param Optional The optional-like object holding a potential value.
     * @return An instance of type T. If the optional-like object contains a value,
     *         it is converted to T using its value. If it does not contain a value,
     *         a default-constructed instance of T is returned.
     *
     * @note The function assumes the presence of `has_value` to check if the
     *       optional-like object holds a value, and `get` to retrieve the value
     *       if present.
     */
    RETROLIB_EXPORT template <OptionalType T, OptionalType F>
        requires std::convertible_to<TTypeParam<F>, TTypeParam<T>>
    constexpr T To(F &&Optional) {
        if (HasValue(Optional)) {
            return T(Get(std::forward<F>(Optional)));
        }

        return T();
    }

    /**
     * Transforms the given input `optional` into a target type using conditional type checks and wrapping mechanisms.
     * If the input is an lvalue reference type and the condition `RawReferenceOptionalValid` is not satisfied,
     * the input is forwarded and transformed into a `std::reference_wrapper` of the raw reference type.
     * Otherwise, the input is directly forwarded and transformed into an instance of `T` with the `TypeParam` deduced
     * type.
     *
     * @param Optional The input element or object being transformed, which can be an rvalue or lvalue. Forwarded
     * accordingly.
     * @return The transformed element of the target type `T`, which depends on the deduced type of the input `optional`
     *         and the internal conditions.
     */
    RETROLIB_EXPORT template <template <typename...> typename T, OptionalType F>
    constexpr auto To(F &&Optional) {
        if constexpr (std::is_lvalue_reference_v<TTypeParam<F>> &&
                      !RawReferenceOptionalValid<T, std::remove_reference_t<TTypeParam<F>>>) {
            return To<T<std::reference_wrapper<std::remove_reference_t<TTypeParam<F>>>>>(std::forward<F>(Optional));
        } else {
            return To<T<TTypeParam<F>>>(std::forward<F>(Optional));
        }
    }

    template <OptionalType T>
    struct TOInvoker {
        template <OptionalType F>
            requires std::convertible_to<TValueType<T>, TValueType<F>>
        constexpr auto operator()(F &&Optional) const {
            return To<T>(std::forward<F>(Optional));
        }
    };

    template <OptionalType T>
    constexpr TOInvoker<T> ToFunction;

    template <template <typename...> typename T>
    struct TTemplatedToInvoker {
        template <OptionalType F>
        constexpr auto operator()(F &&Optional) const {
            return To<T>(std::forward<F>(Optional));
        }
    };

    template <template <typename...> typename T>
    constexpr TTemplatedToInvoker<T> TemplatedToFunction;

    /**
     * @brief Provides an extension method for transforming an object into a specific target type.
     *
     * This function is a constexpr method that invokes an extension method for type conversion.
     * The targeted type of transformation is defined by the template parameter `T`.
     *
     * @tparam T The target type to which the conversion is performed.
     * @return An extension method invoker for handling the conversion to the specified type.
     */
    RETROLIB_EXPORT template <OptionalType T>
    constexpr auto To() {
        return ExtensionMethod<ToFunction<T>>();
    }

    /**
     * @brief Converts an input object to the specified output type using a templated invoker.
     *
     * This method applies the transformation described by the specified templated
     * invoker, allowing seamless conversions between compatible types or structures.
     *
     * @tparam T The target type to which the input object will be converted.
     *
     * @return A result of the conversion performed by the templated invoker, encapsulated
     *         in an extension method for further operations.
     */
    RETROLIB_EXPORT template <template <typename...> typename T>
    constexpr auto To() {
        return ExtensionMethod<TemplatedToFunction<T>>();
    }

} // namespace retro::optionals