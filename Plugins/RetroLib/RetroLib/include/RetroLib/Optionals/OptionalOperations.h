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
        optional.operator->();
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
    concept OptionalType = requires(T &&value) {
        get<T>(std::forward<T>(value));
        get_value<T>(std::forward<T>(value));
        { has_value<T>(std::forward<T>(value)) } -> std::same_as<bool>;
    };

    /**
     * Obtain the type of reference obtained from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using CommonReference = decltype(get(std::declval<T>()));

    /**
     * Obtain the raw value type from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using ValueType = std::decay_t<CommonReference<T>>;

    /**
     * @struct IsOptionalReference
     *
     * @brief A type trait to determine if a type is an optional reference.
     *
     * This structure inherits from `std::false_type` as a default implementation
     * indicating that, by default, a given type is not an optional reference.
     * Specializations of this struct can be created for custom types that
     * represent optional references, allowing compile-time checks or
     * conditional compilation paths dependent on whether a type meets the
     * criteria of being an optional reference.
     */
    template <OptionalType>
    struct IsOptionalReference : std::false_type {};

    /**
     * @brief A type trait to check if a type is an optional reference.
     *
     * This struct is a specialization for types of the form `O<T&>`, where `O` is a template parameter intended
     * to represent optional-like containers and `T` is any type. It inherits from `std::true_type`,
     * indicating that a type is indeed an optional reference.
     *
     * @tparam O A template representing an optional-like type that can be instantiated with a reference.
     * @tparam T The type of the reference for which the optional trait is being checked.
     *
     * This type trait is used to determine if a given type is an optional reference type. It can be used
     * in template metaprogramming to enable or disable functionality based on whether a type is an optional
     * reference.
     */
    template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    struct IsOptionalReference<O<T &>> : std::true_type {};

    /**
     * @brief Trait to determine if a type is an optional reference.
     *
     * This struct is specialized for types that are an optional reference,
     * specifically when the type is `O` holding a `std::reference_wrapper<T>`.
     *
     * @tparam T Type of the object that is referenced.
     */
    template <template <typename...> typename O, typename T>
        requires OptionalType<O<std::reference_wrapper<T>>>
    struct IsOptionalReference<O<std::reference_wrapper<T>>> : std::true_type {};

    /**
     * Concept to check if a type can be converted into an optional reference type.
     */
    RETROLIB_EXPORT template <typename T>
    concept OptionalReference = OptionalType<T> && IsOptionalReference<T>::value;

    /**
     * @struct IsRawReferenceOptionalAllowed
     *
     * @brief A trait to determine if raw references can be considered as optional.
     *
     * This struct inherits from `std::false_type`, indicating that by default
     * raw references are not allowed to be considered optional.
     *
     * Can be specialized for custom types where raw references are conditionally
     * permissible as optional, but the default behavior is to disallow this.
     *
     * @note This is typically used in template metaprogramming to enforce
     * constraints on types or to implement conditional logic based on type traits.
     */
    RETROLIB_EXPORT template <template <typename...> typename>
    struct IsRawReferenceOptionalAllowed : std::false_type {};

    /**
     * Concept to check if making a raw reference to an optional type is valid or not.
     *
     * @tparam O The template type for the optionals
     * @tparam T The contained value type
     */
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
    concept RawReferenceOptionalValid = OptionalType<O<T>> && IsRawReferenceOptionalAllowed<O>::value;

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
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr decltype(auto) make_optional_reference(O<T> &value) {
        if constexpr (OptionalReference<O<T>>) {
            return value;
        } else if constexpr (RawReferenceOptionalValid<O, T>) {
            if (has_value(value)) {
                return O<T &>(*value);
            }

            return O<T &>();
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
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr decltype(auto) make_optional_reference(const O<T> &value) {
        if constexpr (OptionalReference<O<T>>) {
            return value;
        } else if constexpr (RawReferenceOptionalValid<O, T>) {
            if (has_value(value)) {
                return O<const T &>(*value);
            }

            return O<const T &>();
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
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr auto make_optional_reference(O<T> &&value) {
        static_assert(OptionalReference<O<T>>, "Cannot an r-value to an optional reference type.");
        return std::move(value);
    }
}; // namespace retro::optionals