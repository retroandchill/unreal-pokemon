/**
 * @file TypeTraits.h
 * @brief Additional enhancements to the type_traits library
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include <type_traits>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {

    /**
     * @struct FInvalidType
     *
     * @brief Represents an invalid type with a static validity check.
     *
     * This struct is used as a marker to indicate that a certain type or operation is invalid.
     * The static member `IsValid` is always set to `false`, signifying that this type does
     * not meet the necessary criteria or conditions to be considered valid within the context
     * where it's used.
     *
     * @details
     * This struct can be utilized in template metaprogramming to enforce type constraints.
     * It can serve as a default type, representing an invalid or unsupported type scenario,
     * allowing developers to easily determine and handle such cases in their code logic.
     *
     * @see IsValid
     */
    RETROLIB_EXPORT struct FInvalidType {
        static constexpr bool IsValid = false;
    };

    /**
     * @struct ValidType::
     *
     * @brief Represents a type that is confirmed to be valid.
     *
     * This structure contains a static constant expression that is used
     * to determine the validity of the type. It can be used in template
     * metaprogramming to conditionally compile code based on the validity
     * of the type.
     */
    RETROLIB_EXPORT struct FValidType {
        static constexpr bool IsValid = true;
    };

    /**
     * @brief A concept that checks if a type can be dereferenced.
     *
     * This concept ensures that the provided type supports the dereference
     * operator (*). It is used to determine if a given type behaves like a
     * pointer or an iterator by requiring the type to be dereferenceable.
     *
     * @tparam T The type to check for the dereference capability.
     */
    template <typename T>
    concept Dereferenceable = requires(T &&Ptr) { *Ptr; };

    /**
     * Get the underlying type that comes from the pointer dereference.
     *
     * @tparam T The dereferencable type
     */
    template <Dereferenceable T>
    using TDereferencedType = decltype(*std::declval<T>());

    /**
     * @brief Concept to determine if a type can be dereferenced to a specific type reference.
     *
     * This concept checks whether an instance of type T, when dereferenced,
     * yields a result that is convertible to a reference of type C. It is used
     * to constrain template parameters to types that support dereferencing to
     * a specific target type.
     *
     * @tparam T The type to be dereferenced.
     * @tparam C The target type to which the dereferenced value should be convertible.
     */
    template <typename T, typename C>
    concept DereferenceableTo = requires(T &&Ptr) {
        { *Ptr } -> std::convertible_to<C &>;
    };

    RETROLIB_EXPORT template <typename>
    struct TTemplateSpecializationType : FInvalidType {};

    RETROLIB_EXPORT template <template <typename...> typename T, typename... A>
    struct TTemplateSpecializationType<T<A...>> : FValidType {
        using Type = T<A...>;

        template <typename... B>
        using TemplateType = T<B...>;
    };

    RETROLIB_EXPORT template <typename T>
    concept TemplateSpecialization = TTemplateSpecializationType<T>::IsValid;

    RETROLIB_EXPORT template <typename, template <typename...> typename>
    struct IsSpecializationOf : std::false_type {};

    RETROLIB_EXPORT template <template <typename...> typename T, typename... A>
    struct IsSpecializationOf<T<A...>, T> : std::true_type {};

    RETROLIB_EXPORT template <typename T, template <typename...> typename C>
    concept SpecializationOf = IsSpecializationOf<T, C>::value;

    template <bool Condition, typename T>
    using TMaybeConst = std::conditional_t<Condition, const T, T>;

} // namespace retro
