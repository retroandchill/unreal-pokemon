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

namespace Retro::Optionals {

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
     * @class OptionalOperations
     * @brief A structure representing operations that may be optionally provided.
     *
     * This class inherits from InvalidType. It is designed to represent optional
     * operations or functionality that may not always be required or defined.
     *
     * @note The base class `InvalidType` must be defined for the functionality of
     * this class to be used effectively.
     *
     */
    RETROLIB_EXPORT template <typename>
    struct OptionalOperations : InvalidType {};

    /**
     * @brief The OptionalOperations struct provides utility functions to interact with objects of optional-like types.
     *
     * This struct is designed to handle operations on objects that adhere to the STL optional concept. The template
     * parameter T specifies the type that the optional is expected to contain.
     *
     * @tparam T The type of value expected to be managed by the optional-like object.
     */
    RETROLIB_EXPORT template <StlOptional T>
    struct OptionalOperations<T> : ValidType {

        /**
         * @brief Retrieves the value from an optional object by forwarding the input object.
         *
         * This version uses the deference operator, which does perform any checks for the validity of the value leading
         * to undefined behavior.
         *
         * @param Optional An optional object from which the value is to be retrieved.
         *                 The parameter is forwarded, maintaining its value category
         *                 (lvalue or rvalue).
         * @return The value contained in the optional object. The return type maintains
         *         the same value category as the input parameter.
         */
        template <StlOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr decltype(auto) Get(O &&Optional) {
            return *std::forward<O>(Optional);
        }

        /**
         * @brief Retrieves the value from an optional object by forwarding the input object.
         *
         * This version is checked, and will throw if there is no value. If performance is critical do not use this
         * method.
         *
         * @param Optional An optional object from which the value is to be retrieved.
         *                 The parameter is forwarded, maintaining its value category
         *                 (lvalue or rvalue).
         * @return The value contained in the optional object. The return type maintains
         *         the same value category as the input parameter.
         * @throws std::bad_optional_access If the optional does not contain a value
         */
        template <StlOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr decltype(auto) GetValue(O &&Optional) {
            return std::forward<O>(Optional).value();
        }

        /**
         * Checks if the given optional object contains a value.
         *
         * @param Optional An object of type O which must have a has_value() member function.
         * @return A boolean value that is true if the optional object contains a value, otherwise false.
         */
        template <StlOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr bool HasValue(const O &Optional) {
            return Optional.has_value();
        }
    };

    /**
     * Concept used to check if a type is an optional type as defined by the get() and has_value() methods.
     *
     * @tparam T The type to check inside the optional
     */
    RETROLIB_EXPORT template <typename T>
    concept OptionalType = OptionalOperations<std::decay_t<T>>::IsValid && requires(T &&Value) {
        OptionalOperations<std::decay_t<T>>::Get(std::forward<T>(Value));
        OptionalOperations<std::decay_t<T>>::GetValue(std::forward<T>(Value));
        { OptionalOperations<std::decay_t<T>>::HasValue(std::forward<T>(Value)) } -> std::same_as<bool>;
    };

    /**
     * @brief Retrieves the value from an optional object by forwarding the input object.
     *
     * This version uses the deference operator, which does perform any checks for the validity of the value leading to
     * undefined behavior.
     *
     * @param Optional An optional object from which the value is to be retrieved.
     *                 The parameter is forwarded, maintaining its value category
     *                 (lvalue or rvalue).
     * @return The value contained in the optional object. The return type maintains
     *         the same value category as the input parameter.
     */
    RETROLIB_EXPORT template <OptionalType O>
    constexpr decltype(auto) Get(O &&Optional) {
        return OptionalOperations<std::decay_t<O>>::Get(std::forward<O>(Optional));
    }

    /**
     * @brief Retrieves the value from an optional object by forwarding the input object.
     *
     * This version is checked, and will throw if there is no value. If performance is critical do not use this method.
     *
     * @param Optional An optional object from which the value is to be retrieved.
     *                 The parameter is forwarded, maintaining its value category
     *                 (lvalue or rvalue).
     * @return The value contained in the optional object. The return type maintains
     *         the same value category as the input parameter.
     * @throws std::bad_optional_access If the optional does not contain a value
     */
    RETROLIB_EXPORT template <OptionalType O>
    constexpr decltype(auto) GetValue(O &&Optional) {
        return OptionalOperations<std::decay_t<O>>::GetValue(std::forward<O>(Optional));
    }

    /**
     * Checks if the given optional object contains a value.
     *
     * @param Optional An object of type O which must have a has_value() member function.
     * @return A boolean value that is true if the optional object contains a value, otherwise false.
     */
    RETROLIB_EXPORT template <OptionalType O>
    constexpr bool HasValue(const O &Optional) {
        return OptionalOperations<std::decay_t<O>>::HasValue(Optional);
    }

    /**
     * Obtain the type of reference obtained from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using CommonReference = decltype(OptionalOperations<std::decay_t<T>>::Get(std::declval<T>()));

    /**
     * Obtain the raw value type from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using ValueType = std::decay_t<CommonReference<T>>;

    /**
     * @brief Represents a structure for optional parameters.
     *
     * The OptionalParameters structure is expected to hold a set of optional
     * parameters that can be used in various contexts, though the exact use-case
     * depends on specific implementation. It inherits from InvalidType.
     *
     * @note The base class InvalidType must be defined in the relevant context,
     * and the behavior of this structure is dependent on the implementation
     * details and use of the base class.
     *
     * @warning Ensure that the base class InvalidType is properly defined
     * before using this structure to avoid compilation or runtime errors.
     */
    template <typename>
    struct OptionalParameters : InvalidType {};

    /**
     * @struct OptionalParameters
     *
     * @tparam O<T>
     * Template parameter representing a container or a wrapper type that holds a type T.
     *
     * @brief Provides metadata regarding optional parameters.
     * This struct defines the contained type T for a given container O and validates its type.
     */
    template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    struct OptionalParameters<O<T>> : ValidType {
        using ContainedType = T;
    };

    /**
     * @struct OptionalParameters
     * @brief A structure template specialized for std::reference_wrapper<T> that defines additional type traits.
     *
     * This specialization of OptionalParameters is used to manage optional parameters that are wrapped
     * in std::reference_wrapper. It derives from ValidType to ensure the type conforms to certain requirements.
     *
     * @tparam T A type that is wrapped inside std::reference_wrapper for optional parameter handling.
     *
     * @type ContainedType Represents the unwrapped type as a reference (T&). It is the type held within
     *                      the std::reference_wrapper object.
     */
    template <template <typename...> typename O, typename T>
        requires OptionalType<O<std::reference_wrapper<T>>>
    struct OptionalParameters<O<std::reference_wrapper<T>>> : ValidType {
        using ContainedType = T &;
    };

    /**
     * Type parameter definition for the effective type parameter of an optional type. Decaying std::reference_wrapper
     * to a raw reference.
     *
     * @tparam T The optional type in question
     */
    RETROLIB_EXPORT template <OptionalType T>
        requires OptionalParameters<std::decay_t<T>>::IsValid
    using TypeParam = typename OptionalParameters<std::decay_t<T>>::ContainedType;

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
     * @param Value An object of type `O<T>` that is to be conditionally wrapped.
     * @return An object of type `O<T>` if it is already an optional reference, or
     *         an object of type `O<std::reference_wrapper<T>>` if not.
     */
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr decltype(auto) MakeOptionalReference(O<T> &Value) {
        if constexpr (OptionalReference<O<T>>) {
            return Value;
        } else if constexpr (RawReferenceOptionalValid<O, T>) {
            if (HasValue(Value)) {
                return O<T &>(*Value);
            }

            return O<T &>();
        } else {
            if (HasValue(Value)) {
                return O<std::reference_wrapper<T>>(*Value);
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
     * @param Value The optional object from which to make an optional reference.
     * @return A new optional that either contains a reference wrapper to the original value or is
     *         unengaged.
     */
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr decltype(auto) MakeOptionalReference(const O<T> &Value) {
        if constexpr (OptionalReference<O<T>>) {
            return Value;
        } else if constexpr (RawReferenceOptionalValid<O, T>) {
            if (HasValue(Value)) {
                return O<const T &>(*Value);
            }

            return O<const T &>();
        } else {
            if (HasValue(Value)) {
                return O<std::reference_wrapper<const T>>(*Value);
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
     * @param Value The r-value of type `O<T>` to be used in creating an optional reference.
     * @return The result of calling `std::move` on the input value.
     */
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    constexpr auto MakeOptionalReference(O<T> &&Value) {
        static_assert(OptionalReference<O<T>>, "Cannot an r-value to an optional reference type.");
        return std::move(Value);
    }

    /**
     * @brief Represents an invalid type for a nullable optional parameter.
     *
     * This struct is used as a placeholder to indicate a type
     * that is considered invalid in the context of nullable optional parameters.
     * It inherits from the InvalidType base class.
     */
    RETROLIB_EXPORT template <typename>
    struct NullableOptionalParam : InvalidType {};

    /**
     * @struct NullableOptionalParam
     * @brief A template structure to handle nullable pointers and transform them into optional types.
     *
     * @tparam T The base type to be wrapped and processed.
     *
     * This struct is designed to handle pointer-to-type conversions,
     * allowing nullable pointers to be represented as an optional type.
     *
     * @note This struct requires the `ValidType` to be the base type.
     * @note The struct uses `std::remove_pointer_t` and `std::add_lvalue_reference_t`
     *       for processing type definitions.
     */
    RETROLIB_EXPORT template <typename T>
        requires std::is_pointer_v<T>
    struct NullableOptionalParam<T> : ValidType {
        using RawType = std::remove_pointer_t<T>;
        using ReferenceType = std::add_lvalue_reference_t<RawType>;

        template <template <typename...> typename O, typename U>
            requires std::assignable_from<ReferenceType, std::remove_pointer_t<U> &> && OptionalType<O<U>>
        static constexpr auto of_nullable(U *ptr) {
            if constexpr (RawReferenceOptionalValid<O, U>) {
                return ptr != nullptr ? O<ReferenceType>(*ptr) : O<ReferenceType>();
            } else {
                return ptr != nullptr ? O<std::reference_wrapper<T>>(*ptr) : O<std::reference_wrapper<T>>();
            }
        }
    };

    /**
     * Verifies that the given optional and type combo allows for a nullability check rather than just passing the
     * value straight into an optional.
     *
     * @tparam T The type check
     * @tparam O The optional to insert into
     */
    RETROLIB_EXPORT template <typename T, template <typename...> typename O>
    concept Nullable = NullableOptionalParam<std::remove_reference_t<T>>::IsValid && requires(T &&value) {
        {
            NullableOptionalParam<std::remove_reference_t<T>>::template of_nullable<O>(std::forward<T>(value))
        } -> OptionalType;
    };
} // namespace retro::optionals