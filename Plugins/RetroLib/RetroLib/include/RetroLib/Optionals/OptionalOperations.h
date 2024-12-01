/**
 * @file OptionalOperations.h
 * @brief Contains optional utility methods for bridging with other libraries that have their own optional types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/TypeTraits.h"

#include <optional>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::optionals {

    /**
     * Concept used to define a type that matches the criteria of an Optional in the STL.
     *
     * @tparam T The type to check against
     */
    template <typename T>
    concept StlOptional = requires(T &&optional) {
        optional.value();
        *optional;
        { optional.has_value() } -> std::same_as<bool>;
    };

    /**
     * @brief Retrieves the value from an optional object by forwarding the input object.
     *
     * This version uses the deference operator, which does perform any checks for the validity of the value leading to
     * undefined behavior.
     *
     * @param optional An optional object from which the value is to be retrieved.
     *                 The parameter is forwarded, maintaining its value category
     *                 (lvalue or rvalue).
     * @return The value contained in the optional object. The return type maintains
     *         the same value category as the input parameter.
     */
    RETROLIB_EXPORT template <StlOptional O>
    constexpr decltype(auto) get(O &&optional) {
        return *std::forward<O>(optional);
    }

    /**
     * @brief Retrieves the value from an optional object by forwarding the input object.
     *
     * This version is checked, and will throw if there is no value. If performance is critical do not use this method.
     *
     * @param optional An optional object from which the value is to be retrieved.
     *                 The parameter is forwarded, maintaining its value category
     *                 (lvalue or rvalue).
     * @return The value contained in the optional object. The return type maintains
     *         the same value category as the input parameter.
     * @throws std::bad_optional_access If the optional does not contain a value
     */
    RETROLIB_EXPORT template <StlOptional O>
    constexpr decltype(auto) get_value(O &&optional) {
        return std::forward<O>(optional).value();
    }

    /**
     * Checks if the given optional object contains a value.
     *
     * @param optional An object of type O which must have a has_value() member function.
     * @return A boolean value that is true if the optional object contains a value, otherwise false.
     */
    RETROLIB_EXPORT template <StlOptional O>
    constexpr bool has_value(const O &optional) {
        return optional.has_value();
    }

    /**
     * Concept used to check if a type is an optional type as defined by the get() and has_value() methods.
     *
     * @tparam T The type to check inside the optional
     */
    RETROLIB_EXPORT template <typename T>
    concept Optional = requires(T &&value) {
        get<T>(std::forward<T>(value));
        get_value<T>(std::forward<T>(value));
        { has_value<T>(std::forward<T>(value)) } -> std::same_as<bool>;
    };

    /**
     * Obtain the type of reference obtained from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <Optional T>
    using CommonReference = decltype(get(std::declval<T>()));

    /**
     * Obtain the raw value type from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <Optional T>
    using ValueType = std::decay_t<CommonReference<T>>;

    /**
     * @brief A traits structure to determine properties related to optional types.
     *
     * This structure is designed to provide information about specific characteristics
     * of optional types, particularly in the context of template specialization. By default,
     * the struct specifies that a type is not an optional reference.
     */
    template <Optional>
    struct OptionalTraits {
        static constexpr bool is_optional_reference = false;
    };

    /**
     * @struct OptionalTraits
     *
     * @brief Traits structure used to determine specific characteristics of an optional type.
     *
     * The OptionalTraits structure is used to inspect the properties of a given optional type
     * and provides compile-time constant expressions to facilitate conditional logic based on
     * these properties. This particular instantiation is designed for types wrapped in an
     * optional-like structure denoted as O<T>.
     *
     * @tparam O A template parameter where O is an optional-like template and T is the
     *              type contained within the optional.
     *              @tparam T The underlying type of the optional
     *
     * @remark This traits structure checks the nature of the type T contained within the
     *         optional structure to determine if it is an lvalue reference.
     */
    template <template <typename> typename O, typename T>
        requires Optional<O<T>>
    struct OptionalTraits<O<T>> {
        /**
         * @brief A compile-time boolean constant that represents whether a type T is an lvalue reference.
         *
         * This variable utilizes the type trait `std::is_lvalue_reference_v` to determine
         * if the given type T is an lvalue reference. It evaluates to true if T is an
         * lvalue reference, otherwise, it evaluates to false. This can be used in template
         * metaprogramming and static assertions to enforce conditions or optimize code
         * paths based on the reference type characteristics of T.
         */
        static constexpr bool is_optional_reference = std::is_lvalue_reference_v<T>;
    };

    /**
     * @struct OptionalTraits
     * @brief Traits class specialization for handling optional reference wrappers.
     *
     * This specialization of the OptionalTraits struct is designed to work with
     * std::reference_wrapper wrapped within a custom optional-like type `O`.
     * It provides compile-time information about the nature of the optional type
     * being a reference.
     *
     * @tparam T The type of the object referenced by std::reference_wrapper.
     *
     * @tparam O The custom optional-like template that wraps the std::reference_wrapper.
     *
     * @note The primary purpose of this specialization is to flag the optional
     * type as holding a reference using the `is_optional_reference` static
     * constexpr member.
     *
     * @see std::reference_wrapper
     */
    template <template <typename> typename O, typename T>
        requires Optional<O<std::reference_wrapper<T>>>
    struct OptionalTraits<O<std::reference_wrapper<T>>> {
        /**
         * @brief A compile-time boolean constant that represents a predetermined truth value.
         *
         * This constant, `is_optional_reference`, is set to true by default, indicating a specific
         * condition or state within the context of its usage. It may be employed as a flag or
         * configuration parameter to influence logic flow, decision making, or optimization in
         * template metaprogramming or other compile-time operations. Its fixed value asserts
         * a truth statement that does not change during execution.
         */
        static constexpr bool is_optional_reference = true;
    };

    /**
     * Concept to check if a type can be converted into an optional reference type.
     */
    RETROLIB_EXPORT template <typename T>
    concept OptionalReference = OptionalTraits<T>::is_optional_reference;

    /**
     * @brief Creates an optional reference wrapper around a given value.
     *
     * This function takes an object of type `O<T>` and returns it directly if it
     * supports being an optional reference based on the `OptionalReference` type trait.
     * Otherwise, it returns an object of type `O<std::reference_wrapper<T>>` which
     * wraps the given value in a `std::reference_wrapper`.
     *
     * @param value An object of type `O<T>` that is to be conditionally wrapped.
     * @return An object of type `O<T>` if it is already an optional reference, or
     *         an object of type `O<std::reference_wrapper<T>>` if not.
     */
    RETROLIB_EXPORT template <template <typename> typename O, typename T>
        requires Optional<O<T>>
    constexpr decltype(auto) make_optional_reference(O<T> &value) {
        if constexpr (OptionalReference<O<T>>) {
            return value;
        } else {
            if (has_value(value)) {
                return O<std::reference_wrapper<T>>(*value);
            }

            return O<std::reference_wrapper<T>>();
        }
    }

    /**
     * @brief Creates an optional reference from an optional object.
     *
     * This function template takes an optional object `value` of type `O<T>`. If the type satisfies
     * the `OptionalReference` trait, it returns the object as is. Otherwise, it checks if the
     * optional contains a value using `has_value`. If it does, it returns an optional containing a
     * reference wrapper to the value. If not, it returns an optional reference wrapper in an
     * unengaged state.
     *
     * @tparam O A template parameter representing an optional type.
     * @tparam T The type of the contained value within the optional.
     * @param value The optional object from which to make an optional reference.
     * @return A new optional that either contains a reference wrapper to the original value or is
     *         unengaged.
     */
    RETROLIB_EXPORT template <template <typename> typename O, typename T>
        requires Optional<O<T>>
    constexpr decltype(auto) make_optional_reference(const O<T> &value) {
        if constexpr (OptionalReference<O<T>>) {
            return value;
        } else {
            if (has_value(value)) {
                return O<std::reference_wrapper<const T>>(*value);
            }

            return O<std::reference_wrapper<const T>>();
        }
    }

    /**
     * @brief Creates an optional reference from an r-value of type `O<T>`.
     *
     * This function template attempts to convert a given r-value `value` of
     * type `O<T>` into an optional reference. It employs a static assertion
     * to ensure that `O<T>` satisfies the `OptionalReference` type trait.
     * If the assertion is not met, a compilation error will occur indicating
     * that an r-value cannot be assigned to an optional reference type.
     *
     * @tparam O Template parameter representing the type encapsulated by the optional reference.
     * @tparam T Template parameter denoting the type held by `O`.
     * @param value The r-value of type `O<T>` to be used in creating an optional reference.
     * @return The result of calling `std::move` on the input value.
     */
    RETROLIB_EXPORT template <template <typename> typename O, typename T>
        requires Optional<O<T>>
    constexpr auto make_optional_reference(O<T> &&value) {
        static_assert(OptionalReference<O<T>>, "Cannot an r-value to an optional reference type.");
        return std::move(value);
    }
}; // namespace retro::optionals