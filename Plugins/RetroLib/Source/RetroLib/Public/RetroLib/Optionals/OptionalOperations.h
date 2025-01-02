/**
 * @file OptionalOperations.h
 * @brief Contains optional utility methods for bridging with other libraries that have their own optional types.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/RetroLibMacros.h"
#include <optional>
#endif

#include "RetroLib/TypeTraits.h"
#include "RetroLib/Utils/ForwardLike.h"



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
     * @class TOptionalOperations
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
    struct TOptionalOperations : FInvalidType {};

    /**
     * @brief The OptionalOperations struct provides utility functions to interact with objects of optional-like types.
     *
     * This struct is designed to handle operations on objects that adhere to the STL optional concept. The template
     * parameter T specifies the type that the optional is expected to contain.
     *
     * @tparam T The type of value expected to be managed by the optional-like object.
     */
    RETROLIB_EXPORT template <StlOptional T>
    struct TOptionalOperations<T> : FValidType {

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
    concept OptionalType = TOptionalOperations<std::decay_t<T>>::IsValid && requires(T &&Value) {
        TOptionalOperations<std::decay_t<T>>::Get(std::forward<T>(Value));
        TOptionalOperations<std::decay_t<T>>::GetValue(std::forward<T>(Value));
        { TOptionalOperations<std::decay_t<T>>::HasValue(std::forward<T>(Value)) } -> std::same_as<bool>;
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
        return TOptionalOperations<std::decay_t<O>>::Get(std::forward<O>(Optional));
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
        return TOptionalOperations<std::decay_t<O>>::GetValue(std::forward<O>(Optional));
    }

    /**
     * Checks if the given optional object contains a value.
     *
     * @param Optional An object of type O which must have a has_value() member function.
     * @return A boolean value that is true if the optional object contains a value, otherwise false.
     */
    RETROLIB_EXPORT template <OptionalType O>
    constexpr bool HasValue(const O &Optional) {
        return TOptionalOperations<std::decay_t<O>>::HasValue(Optional);
    }

    /**
     * Obtain the type of reference obtained from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using TCommonReference = decltype(TOptionalOperations<std::decay_t<T>>::Get(std::declval<T>()));

    /**
     * Obtain the raw value type from the optional in question.
     *
     * @tparam T The optional to dereference
     */
    RETROLIB_EXPORT template <OptionalType T>
    using TValueType = std::decay_t<TCommonReference<T>>;

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
    struct TOptionalParameters : FInvalidType {};

    /**
     * @struct TOptionalParameters
     *
     * @tparam O<T>
     * Template parameter representing a container or a wrapper type that holds a type T.
     *
     * @brief Provides metadata regarding optional parameters.
     * This struct defines the contained type T for a given container O and validates its type.
     */
    template <template <typename...> typename O, typename T>
        requires OptionalType<O<T>>
    struct TOptionalParameters<O<T>> : FValidType {
        using ContainedType = T;
    };

    /**
     * @struct TOptionalParameters
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
    struct TOptionalParameters<O<std::reference_wrapper<T>>> : FValidType {
        using ContainedType = T &;
    };

    /**
     * Type parameter definition for the effective type parameter of an optional type. Decaying std::reference_wrapper
     * to a raw reference.
     *
     * @tparam T The optional type in question
     */
    RETROLIB_EXPORT template <OptionalType T>
        requires TOptionalParameters<std::decay_t<T>>::IsValid
    using TTypeParam = typename TOptionalParameters<std::decay_t<T>>::ContainedType;

    /**
     * @struct TIsOptionalReference
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
    struct TIsOptionalReference : std::false_type {};

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
    struct TIsOptionalReference<O<T &>> : std::true_type {};

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
    struct TIsOptionalReference<O<std::reference_wrapper<T>>> : std::true_type {};

    /**
     * Concept to check if a type can be converted into an optional reference type.
     */
    RETROLIB_EXPORT template <typename T>
    concept OptionalReference = OptionalType<T> && TIsOptionalReference<T>::value;

    /**
     * @struct TIsRawReferenceOptionalAllowed
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
    struct TIsRawReferenceOptionalAllowed : std::false_type {};

    /**
     * Concept to check if making a raw reference to an optional type is valid or not.
     *
     * @tparam O The template type for the optionals
     * @tparam T The contained value type
     */
    RETROLIB_EXPORT template <template <typename...> typename O, typename T>
    concept RawReferenceOptionalValid = OptionalType<O<T>> && TIsRawReferenceOptionalAllowed<O>::value;

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
    struct TNullableOptionalParam : FInvalidType {};

    /**
     * @struct TNullableOptionalParam
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
        requires std::is_pointer_v<T> && (!std::is_void_v<std::remove_pointer_t<T>>)
    struct TNullableOptionalParam<T> : FValidType {
        using RawType = std::remove_pointer_t<T>;
        using ReferenceType = std::add_lvalue_reference_t<RawType>;

        template <template <typename...> typename O, typename U>
            requires std::convertible_to<ReferenceType, std::remove_pointer_t<U> &> && OptionalType<O<std::reference_wrapper<U>>>
        static constexpr auto OfNullable(U *Ptr) {
            if constexpr (RawReferenceOptionalValid<O, U>) {
                return Ptr != nullptr ? O<ReferenceType>(*Ptr) : O<ReferenceType>();
            } else {
                return Ptr != nullptr ? O<std::reference_wrapper<std::remove_pointer_t<T>>>(*Ptr) : O<std::reference_wrapper<std::remove_pointer_t<T>>>();
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
    concept Nullable = TNullableOptionalParam<std::remove_reference_t<T>>::IsValid && requires(T &&Value) {
        {
            TNullableOptionalParam<std::remove_reference_t<T>>::template OfNullable<O>(std::forward<T>(Value))
        } -> OptionalType;
    };

    /**
     * @struct FOptionalSentinel
     *
     * Sentinel value for iterating through an optional value. Represents having reached the end of iteration.
     */
    RETROLIB_EXPORT struct FOptionalSentinel {};

    /**
     * @struct TOptionalIterator
     *
     * Iterator type for iterating through an optional, as if it were a range. If the optional is empty, the range is
     * considered to be empty.
     *
     * @tparam O The type of optional that is contained.
     */
    RETROLIB_EXPORT template <OptionalType O>
    struct TOptionalIterator {
        using value_type = TValueType<O>;
        using difference_type = std::ptrdiff_t;

        /**
         * @brief Default constructor for `OptionalIterator`.
         *
         * Constructs an `OptionalIterator` instance with a default state. This
         * ensures that the object is initialized appropriately without requiring
         * any specific argument for construction.
         *
         * @return A new `OptionalIterator` object with a default state.
         */
        constexpr TOptionalIterator() = default;

        /**
         * @brief Constructs an `OptionalIterator` with a reference to a value.
         *
         * Creates an instance of `OptionalIterator` by explicitly passing a reference
         * to an object to manage. This ensures the iterator has a defined value it can
         * access and operate on.
         *
         * @param Value A reference to the object this iterator will manage.
         */
        constexpr explicit TOptionalIterator(O &Value) : Value(&Value) {
        }

        /**
         * @brief Dereference operator for accessing the stored value.
         *
         * Provides access to the value stored within the object. This operator
         * asserts that the stored pointer is not null and contains a valid value
         * before dereferencing it.
         *
         * @return A constant reference or value of the dereferenced stored object.
         */
        constexpr decltype(auto) operator*() const {
            RETROLIB_ASSERT(Value != nullptr && HasValue(*Value));
            return **Value;
        }

        /**
         * @brief Equality comparison operator for `OptionalIterator`.
         *
         * Compares this `OptionalIterator` instance with another to determine if they
         * are equal based on their internal values.
         *
         * @param Other The `OptionalIterator` instance to compare against.
         * @return `true` if both `OptionalIterator` instances have equal values,
         *         otherwise `false`.
         */
        constexpr bool operator==(const TOptionalIterator &Other) const {
            return Value == Other.Value;
        }

        /**
         * @brief Equality operator for `OptionalSentinel`.
         *
         * Compares the current `OptionalSentinel` with another instance to check
         * if they are considered equal. Equality is determined based on the value
         * state or if the value does not satisfy the expected condition.
         *
         * @return `true` if the instances are considered equal, `false` otherwise.
         */
        constexpr bool operator==(const FOptionalSentinel &) const {
            return Value == nullptr || !HasValue(*Value);
        }

        /**
         * @brief Increment operator for `OptionalIterator`.
         *
         * Advances the state of the `OptionalIterator` by nullifying its value. Ensures
         * internal consistency by asserting that the current value is not null before
         * performing the operation.
         *
         * @return A reference to the updated `OptionalIterator` object.
         */
        constexpr TOptionalIterator &operator++() {
            RETROLIB_ASSERT(Value != nullptr);
            Value = nullptr;
            return *this;
        }

        /**
         * @brief Post-increment operator for `OptionalIterator`.
         *
         * Creates a copy of the current `OptionalIterator` instance, increments the
         * current instance, and returns the copy. This operator allows the iterator
         * to be incremented in the post-increment style.
         *
         * @return A copy of the `OptionalIterator` instance before it was incremented.
         */
        constexpr TOptionalIterator operator++(int) {
            TOptionalIterator Copy = *this;
            ++*this;
            return Copy;
        }

    private:
        O *Value = nullptr;
    };

#ifdef __UNREAL__
    RETROLIB_EXPORT template <typename T>
    concept UnrealOptional = requires(T &&Optional) {
        Optional.GetValue();
        *Optional;
        Optional.operator->();
        { Optional.IsSet() } -> std::same_as<bool>;
    };

    template <typename>
    struct TUeOptional : FInvalidType {};

    template <typename T>
    struct TUeOptional<TOptional<T>> : FValidType {
        using Type = T;
    };

    template <typename T>
        requires TUeOptional<std::decay_t<T>>::IsValid
    using TUeOptionalType = typename TUeOptional<std::decay_t<T>>::Type;
    

    RETROLIB_EXPORT template <UnrealOptional T>
    struct TOptionalOperations<T> : FValidType {
        template <UnrealOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr decltype(auto) Get(O &&Optional) {
            if constexpr (std::is_lvalue_reference_v<TUeOptionalType<O>>) {
                return *Optional;
            } else {
                return Retro::ForwardLike<O>(*Optional);
            }
        }

        template <UnrealOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr decltype(auto) GetValue(O &&Optional) {
            if constexpr (std::is_lvalue_reference_v<TUeOptionalType<O>>) {
                return Optional.GetValue();
            } else {
                return Retro::ForwardLike<O>(Optional.GetValue());
            }
        }

        template <UnrealOptional O>
            requires std::same_as<T, std::decay_t<O>>
        static constexpr bool HasValue(const O &Optional) {
            return Optional.IsSet();
        }
    };

    RETROLIB_EXPORT template <typename T>
        requires std::derived_from<std::remove_cv_t<T>, UObject>
    struct TNullableOptionalParam<TObjectPtr<T>> : FValidType {
        using RawType = T;
        using ReferenceType = std::add_lvalue_reference_t<T>;

        template <template <typename...> typename O, typename U>
            requires std::assignable_from<ReferenceType, U&> && OptionalType<O<std::reference_wrapper<U>>>
        static constexpr auto OfNullable(const TObjectPtr<U> &Ptr) {
            if constexpr (RawReferenceOptionalValid<O, U>) {
                return Ptr != nullptr ? O<ReferenceType>(*Ptr) : O<ReferenceType>();
            } else {
                return Ptr != nullptr ? O<std::reference_wrapper<T>>(*Ptr) : O<std::reference_wrapper<T>>();
            }
        }
    };

    RETROLIB_EXPORT template <typename T>
        requires SpecializationOf<std::decay_t<T>, TObjectPtr> && (!std::same_as<std::decay_t<T>, T>)
    struct TNullableOptionalParam<T> : TNullableOptionalParam<std::decay_t<T>> {
    };
}

/**
 * Template specialization for an optional that takes in a reference.
 * @tparam T The type of the optional that was consumed.
 */
RETROLIB_EXPORT template <typename T>
struct TOptional<T &> {

    /**
     * Typedef for the element type used by this container.
     */
    using ElementType = T;

    /**
     * Default constructor, creates an empty optional.
     */
    constexpr TOptional() = default;

    /**
     * Construct a new optional from a reference.
     * @param Value The value to reference in this optional.
     */
    constexpr explicit(false) TOptional(T &Value) : Data(&Value) {
    }

    /**
     * Construct a new optional from a nullable pointer.
     * @param Value
     */
    constexpr explicit(false) TOptional(T *Value) : Data(Value) {
    }

    /**
     * Constructor from nullptr
     */
    constexpr explicit(false) TOptional(nullptr_t) {
    }

    /**
     * Construct a new optional from an optional of subclass.
     * @tparam U The subclass type
     * @param Other The other optional
     */
    template <typename U>
        requires std::derived_from<U, T> && (!std::is_same_v<T, U>)
    constexpr explicit(false) TOptional(TOptional<U &> Other) : Data(Other.GetPtrOrNull()) {
    }

    /**
     * Assignment operator from the underlying value.
     * @param Value The value to assign in.
     * @return A reference to this object
     */
    TOptional &operator=(T &Value) {
        Data = &Value;
        return *this;
    }

    /**
     * Emplace a new reference value into the optional.
     * @param Value The value to emplace
     * @return A reference to the emplaced value
     */
    T &Emplace(T &Value) {
        Data = Value;
        return *Data;
    }

    /**
     * Get the value of the optional.
     * @param DefaultValue The default to substitute for an empty optional.
     * @return The retrieved value
     */
    T &Get(T &DefaultValue) const
        requires(!std::is_const_v<T>)
    {
        return Data != nullptr ? *Data : DefaultValue;
    }

    /**
     * Get the value of the optional.
     * @param DefaultValue The default to substitute for an empty optional.
     * @return The retrieved value
     */
    const T &Get(const T &DefaultValue) const {
        return Data != nullptr ? *Data : DefaultValue;
    }

    /**
     * Get a nullable pointer for the optional.
     * @return The retrieved value
     */
    T *GetPtrOrNull() const {
        return Data;
    }

    /**
     * Get a reference to the underlying data.
     * @return The retrieved value
     */
    T &GetValue() const {
        check(Data != nullptr) return *Data;
    }

    /**
     * Returns if the value is set
     * @return Is there a valid optional?
     */
    bool IsSet() const {
        return Data != nullptr;
    }

    /**
     * Reset the data to empty.
     */
    void Reset() {
        Data = nullptr;
    }

    /**
     * Dereference operator.
     * @return A reference to the underlying data.
     */
    T &operator*() {
        check(Data != nullptr) return *Data;
    }

    /**
     * Dereference operator.
     * @return A reference to the underlying data.
     */
    T &operator*() const {
        check(Data != nullptr) return *Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T *operator->() {
        check(Data != nullptr) return Data;
    }

    /**
     * Pointer member access operator.
     * @return A pointer to the underlying data.
     */
    T *operator->() const {
        check(Data != nullptr) return Data;
    }

  private:
    T *Data = nullptr;
};

RETROLIB_EXPORT template <>
struct Retro::Optionals::TIsRawReferenceOptionalAllowed<TOptional> : std::true_type {};

template <typename T>
constexpr auto begin(TOptional<T>& Optional) {
    return Retro::Optionals::TOptionalIterator(Optional);
}

template <typename T>
constexpr auto begin(const TOptional<T>& Optional) {
    return Retro::Optionals::TOptionalIterator(Optional);
}

template <typename T>
[[noreturn]] constexpr auto begin(TOptional<T>&&) {
    static_assert(false, "Cannot iterate over an r-value TOptional value");
}

template <typename T>
constexpr auto end(const TOptional<T>&) {
    return Retro::Optionals::FOptionalSentinel();
}

template <typename T>
constexpr size_t size(const TOptional<T>& Optional) {
    return Optional.IsSet() ? 1 : 0;
}

namespace Retro::Optionals {
#endif

    /**
     * Constructs an OptionalIterator by deducing the template type from the provided object.
     *
     * @param O The object from which the OptionalIterator's template type is deduced.
     *          The type is determined by removing the reference from the provided object.
     * @return An OptionalIterator instance with the deduced template type.
     */
    RETROLIB_EXPORT template <OptionalType O>
    TOptionalIterator(O &&) -> TOptionalIterator<std::remove_reference_t<O>>;

    /**
         * @brief Creates an object of type O by encapsulating a given value.
         *
         * Constructs an instance of O using the provided value, which is forwarded
         * to maintain its value category. The type of the encapsulated value is
         * deduced and decayed to remove any reference or const/volatile qualifiers.
         *
         * @tparam T The type of the value to be encapsulated.
         * @param Value The value to be wrapped in an O object.
         * @return A new instance of O containing the forwarded value.
         */
        RETROLIB_EXPORT template <template <typename...> typename O = RETROLIB_DEFAULT_OPTIONAL_TYPE, typename T>
            requires OptionalType<O<std::decay_t<T>>>
        constexpr O<std::decay_t<T>> Of(T &&Value) {
            return O<std::decay_t<T>>(std::forward<T>(Value));
        }

        /**
         * @brief Creates an instance of the optional type with a reference to the provided value.
         *
         * This function returns an instance of the optional type `O` containing a reference
         * to the specified value. The behavior depends on whether the combination of `O` and
         * `T` satisfies the `RawReferenceOptionalValid` constraint:
         * - If valid, the result is `O<T&>` initialized with the provided reference.
         * - Otherwise, the result is `O<std::reference_wrapper<T>>` wrapping the reference.
         *
         * The implementation uses `constexpr` to ensure this decision can occur at compile time.
         *
         * @tparam T The type of the value to be wrapped as a reference.
         * @param Value A reference to the value to be stored in the optional.
         * @return An instance of the optional type `O` containing the reference to the value.
         */
        RETROLIB_EXPORT template <template <typename...> typename O = RETROLIB_DEFAULT_OPTIONAL_TYPE, typename T>
            requires OptionalType<O<std::decay_t<T>>>
        constexpr auto OfReference(T &Value) {
            if constexpr (RawReferenceOptionalValid<O, T>) {
                return O<T &>(Value);
            } else {
                return O<std::reference_wrapper<T>>(Value);
            }
        }

        /**
         * @brief Creates a nullable optional parameter from the given value.
         *
         * Constructs a NullableOptionalParam object by forwarding the provided value.
         * It utilizes the `of_nullable` function template from `NullableOptionalParam`
         * and ensures the correct type by using `std::remove_reference_t` on the input value.
         * This function is marked as constexpr, allowing it to be evaluated at compile time.
         *
         * @tparam T The type of the input value, which may be a reference or rvalue.
         * @param Value The value to be used for creating the nullable optional parameter.
         * @return A NullableOptionalParam object containing the nullable value.
         */
        RETROLIB_EXPORT template <template <typename...> typename O = RETROLIB_DEFAULT_OPTIONAL_TYPE, typename T>
            requires Nullable<T, O>
        constexpr auto OfNullable(T &&Value) {
            return TNullableOptionalParam<std::remove_reference_t<T>>::template OfNullable<O>(std::forward<T>(Value));
        }
} // namespace retro::optionals