/**
 * @file Optional.h
 * @brief Custom optional type that has support for reference-based optionals.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Concepts/Operators.h"
#include "RetroLib/Concepts/ParameterPacks.h"
#include "RetroLib/Optionals/OptionalIterator.h"
#include "RetroLib/RetroLibMacros.h"

#include <utility>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro {
    /**
     * @struct IsValidOptionalType
     * @brief A type trait used to determine if a type is a valid optional type.
     *
     * This struct inherits from std::true_type, indicating that the default
     * case for this trait is true. It provides a mechanism to validate
     * types as optional, allowing for compile-time checks.
     *
     * Users can extend or specialize this struct to modify the behavior
     * for specific types, ensuring that custom types can be identified as
     * valid optional types when necessary.
     *
     * @note Specialize this struct for any custom type that should be
     *       considered as a valid optional type in your implementation.
     */
    template <typename>
    struct IsValidOptionalType : std::true_type {};

    /**
     * @struct IsValidOptionalType
     *
     * @brief Trait class template specialization to determine the validity of a type as an optional.
     *
     * This specialization of the `IsValidOptionalType` template is specifically for handling
     * `std::nullopt_t`, a type used to represent disengaged states in optional objects.
     * For this specialization, it derives from `std::false_type`, indicating that
     * `std::nullopt_t` is not a valid type for an optional object in this context.
     *
     * @tparam std::nullopt_t Specialized parameter representing the type indicating
     *         an uninitialized or empty state in optional objects.
     */
    template <>
    struct IsValidOptionalType<std::nullopt_t> : std::false_type {};

    /**
     * @brief Struct template specialization to define the validity of an optional type.
     *
     * This specialization of the `IsValidOptionalType` struct template is used to
     * mark `std::in_place_type_t<T>` as an invalid optional type. It inherits from
     * `std::false_type`, effectively indicating a logical "false" for checks that
     * rely on this struct to determine type validity.
     *
     * @tparam T The type encapsulated by `std::in_place_type_t`.
     */
    template <typename T>
    struct IsValidOptionalType<std::in_place_type_t<T>> : std::false_type {};

    /**
     * Concept to check if a type is allowed to be used in an optional.
     *
     * @tparam T The target type
     */
    RETROLIB_EXPORT template <typename T>
    concept ValidOptionalType = IsValidOptionalType<std::decay_t<T>>::value && std::destructible<T> &&
                                (std::is_object_v<T> || std::is_lvalue_reference_v<T>);

    RETROLIB_EXPORT template <ValidOptionalType T>
    class Optional;

    /**
     * Throws a std::bad_optional_access exception.
     * This function is marked [[noreturn]], indicating that it does
     * not return control to the caller.
     *
     * @return This function does not return a value because it always throws an exception.
     *         The noreturn attribute serves as a hint to the compiler that the function
     *         will never return normally, allowing for certain optimizations and warnings
     *         regarding control flow to be suppressed.
     */
    template <typename = void>
    [[noreturn]] bool throw_bad_optional_access() {
        throw std::bad_optional_access();
    }

    /**
     * @struct OptionalStorage
     * @brief A structure that provides storage and management for an optional value.
     *
     * This structure uses a union to store either an active value of type T or an indicator
     * that the value is not set. It offers functionalities to construct, store, and reset
     * the optional value.
     */
    template <ValidOptionalType T>
    struct OptionalStorage {
        union {
            std::nullopt_t empty;
            std::remove_cv_t<T> data;
        };

        bool is_set = false;

        /**
         * @brief Default constructor for the OptionalStorage class.
         *
         * Initializes an OptionalStorage object with an empty optional state using
         * std::nullopt. It is a constexpr and noexcept constructor, meaning it can
         * be evaluated at compile time and does not throw exceptions.
         */
        constexpr OptionalStorage() noexcept : empty(std::nullopt) {
        }

        /**
         * @brief Constructor for the Optional class that initializes the object in-place with the provided arguments.
         *
         * This constructor allows for the in-place construction of the stored value of type T using the given
         * arguments. It utilizes std::in_place_type_t to enable this direct initialization and forwards the arguments
         * for construction. The noexcept specification depends on whether the constructor of T, given arguments A...,
         * is noexcept.
         *
         * @param args The arguments to be used for constructing the stored value of type T.
         */
        template <typename... A>
            requires std::constructible_from<T, A...> && (!PackSameAs<OptionalStorage, A...>)
        constexpr explicit OptionalStorage(std::in_place_type_t<T>,
                                           A &&...args) noexcept(std::is_nothrow_constructible_v<T, A...>)
            : data(std::forward<A>(args)...), is_set(true) {
        }

        /**
         * @brief Resets the state to not set.
         *
         * This constexpr function sets the state indicator `is_set` to false, indicating
         * that the optional state is no longer set. It is noexcept, ensuring no exceptions
         * are thrown during execution of the function. This allows it to be safely used in
         * contexts where exception safety is required.
         */
        constexpr void reset() noexcept {
            is_set = false;
        }
    };

    /**
     * @brief Represents a storage mechanism for an optional object.
     *
     * The OptionalStorage class is a templated structure, allowing storage of any
     * data type in an optional state. It uses a union to efficiently manage memory
     * for the contained data or represent an empty state with std::nullopt.
     *
     * @tparam T The type of the object to store.
     */
    template <ValidOptionalType T>
        requires(!std::is_trivially_destructible_v<T>)
    struct OptionalStorage<T> {
        union {
            std::nullopt_t empty;
            std::remove_cv_t<T> data;
        };

        bool is_set = false;

        /**
         * @brief Default constructor for OptionalStorage.
         *
         * Initializes the OptionalStorage object in an empty state using std::nullopt.
         * This constructor is marked as constexpr, allowing it to be evaluated at compile time,
         * and noexcept, ensuring it does not throw exceptions during its execution.
         */
        constexpr OptionalStorage() noexcept : empty(std::nullopt) {
        }

        /**
         * @brief Constructs an Optional object with an in-place value.
         *
         * This constructor initializes the Optional object to hold a value of type T
         * by directly constructing it using the provided arguments. It ensures that the object
         * is in a valid state with the value set.
         *
         * @tparam A Parameter pack representing the types of arguments used to construct T.
         * @param args Arguments forwarded to the constructor of type T.
         * @return No return value, as this is a constructor.
         *
         * @note This constructor is marked constexpr and noexcept. The noexcept specification
         *       depends on whether the construction of T with the provided arguments is
         *       noexcept.
         */
        template <typename... A>
            requires std::constructible_from<T, A...> && (!PackSameAs<std::decay_t<OptionalStorage>, A...>)
        constexpr explicit OptionalStorage(std::in_place_type_t<T>,
                                           A &&...args) noexcept(std::is_nothrow_constructible_v<T, A...>)
            : data(std::forward<A>(args)...), is_set(true) {
        }

        /**
         * @brief Copy constructor for the OptionalStorage class.
         *
         * Provides a default copy constructor that initializes a new OptionalStorage object
         * from an existing one by copying its state. It is a constexpr constructor that can
         * be evaluated at compile time.
         *
         * @param other The OptionalStorage object to copy from.
         * @return A new OptionalStorage object that is a copy of the given object.
         */
        constexpr OptionalStorage(const OptionalStorage &other) = default;

        /**
         * @brief Move constructor for the OptionalStorage class.
         *
         * Constructs an OptionalStorage object by transferring the state from
         * another OptionalStorage instance. This operation is done using the
         * default move constructor, and it is both constexpr and noexcept,
         * ensuring that it can be performed at compile time and that it
         * will not throw exceptions.
         *
         * @param other The OptionalStorage object to move from.
         * @return A new OptionalStorage object with the transferred state.
         */
        constexpr OptionalStorage(OptionalStorage &&other) = default;

        /**
         * @brief Destructor for the OptionalStorage class.
         *
         * Cleans up the resources used by the OptionalStorage object by calling the reset() method.
         * This ensures that any managed resource is released appropriately when the object goes out of scope.
         */
        ~OptionalStorage() {
            reset();
        }

        /**
         * @brief Copy assignment operator for the OptionalStorage class.
         *
         * This operator assigns the state of another OptionalStorage instance to the
         * current instance using the default copy assignment behavior.
         *
         * @param other The other OptionalStorage instance to copy from.
         * @return A reference to the current instance after assignment.
         */
        constexpr OptionalStorage &operator=(const OptionalStorage &other) = default;

        /**
         * @brief Move assignment operator for the OptionalStorage class.
         *
         * Assigns the state of another OptionalStorage object to this object using move semantics.
         * The defaulted implementation ensures optimal performance by transferring resources
         * rather than copying, which does not throw exceptions.
         *
         * @param other The OptionalStorage object to be moved.
         * @return A reference to the current OptionalStorage object after assignment.
         */
        constexpr OptionalStorage &operator=(OptionalStorage &&other) = default;

        /**
         * @brief Resets the stored value, if any, to an uninitialized state.
         *
         * This method checks if there is a value currently stored. If so, it destructs
         * the stored object and marks the storage as not containing a value. This
         * operation is both constexpr and noexcept, meaning it can be evaluated at
         * compile time and does not throw exceptions.
         */
        constexpr void reset() noexcept {
            if (is_set) {
                data.~T();
                is_set = false;
            }
        }
    };

    /**
     * @brief The OptionalBase class provides a base implementation for optional-like storage.
     *
     * It inherits from OptionalStorage<T> and implements essential operations for managing
     * an optional value such as dereferencing, accessing, and swapping. OptionalBase manages
     * an internal state that indicates whether a value is currently stored.
     *
     * @tparam T The type of the value that can be optionally held by this class.
     */
    template <ValidOptionalType T>
    struct OptionalBase : private OptionalStorage<T> {
        using OptionalStorage<T>::OptionalStorage;
        using OptionalStorage<T>::reset;

        /**
         * @brief Default constructor for the OptionalBase class.
         *
         * This constructor initializes an OptionalBase object with its default state.
         * It is a noexcept constructor, ensuring that no exceptions will be thrown during its execution.
         */
        OptionalBase() noexcept = default;

        /**
         * @brief Checks if the OptionalStorage contains a value.
         *
         * This method returns a boolean indicating whether the Optional
         * object currently holds a non-empty value. It is a constexpr and noexcept
         * method, meaning it can be evaluated at compile time and guarantees not
         * to throw exceptions.
         *
         * @return True if the Optional has a value, otherwise false.
         */
        constexpr bool has_value() const noexcept {
            return is_set;
        }

        /**
         * @brief Dereference operator providing access to the stored value.
         *
         * This operator returns a reference to the stored value if the
         * optional state is set. An assertion is in place to ensure that
         * the value is available, which enforces the safety of the dereference.
         *
         * @return A reference to the stored value of type T.
         *
         * @throws If the assertion fails, indicating the value is not set.
         */
        constexpr T &operator*() & noexcept {
            RETROLIB_ASSERT(is_set);
            return data;
        }

        /**
         * @brief Dereference operator for accessing the stored value.
         *
         * Provides access to the value stored within the object, ensuring that
         * the value is set before allowing access. The method is a constexpr and
         * noexcept, meaning it can be evaluated at compile time and does not
         * throw exceptions.
         *
         * @return A constant reference to the stored value of type T.
         */
        constexpr const T &operator*() const & noexcept {
            RETROLIB_ASSERT(is_set);
            return data;
        }

        /**
         * @brief Dereferences and returns a constant reference to the stored value.
         *
         * This operator ensures that the value is set before accessing it. It is a
         * constexpr and noexcept operator, allowing for compile-time evaluation and
         * guaranteeing not to throw exceptions.
         *
         * @return A constant reference to the stored value of type T.
         */
        constexpr T &&operator*() && noexcept {
            RETROLIB_ASSERT(is_set);
            return std::move(data);
        }

        /**
         * @brief Provides access to the stored data.
         *
         * This operator returns a pointer to the stored data of type T. It ensures
         * that the data is valid and safely accessible, throwing an assertion if data
         * is not set. The operator is constexpr and noexcept, indicating it can be
         * evaluated at compile time and does not throw exceptions.
         *
         * @return A pointer to the stored data.
         */
        constexpr T *operator->() & noexcept {
            RETROLIB_ASSERT(is_set);
            return &data;
        }

        /**
         * @brief Provides const access to the stored object via a pointer.
         *
         * This operator returns a pointer to the stored object only if the object
         * is in a set state. It is a constexpr and noexcept operation, ensuring
         * it can be used in constant expressions without throwing exceptions.
         *
         * @return A pointer to the stored object if it is set.
         * @throws Assertion failure if the object is not in a set state.
         */
        constexpr const T *operator->() const & noexcept {
            RETROLIB_ASSERT(is_set);
            return &data;
        }

        /**
         * @brief Exchanges the states of this OptionalBase object with another.
         *
         * This method swaps the state of the current object with the provided `other` object.
         * The swap operation is conditionally noexcept based on whether the type `T`
         * is nothrow move constructible and nothrow swappable. The swap proceeds differently
         * depending on whether the type `T` can be trivially moved and assigned.
         *
         * @param other The OptionalBase object to swap states with.
         */
        constexpr void swap(OptionalBase &other) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                          std::is_nothrow_swappable_v<T>)
            requires std::swappable<T>
        {
            using std::swap;

            constexpr bool can_swap_trivially =
                std::is_trivially_move_constructible_v<T> && std::is_trivially_move_assignable_v<T>;
            if constexpr (can_swap_trivially) {
                swap(static_cast<OptionalStorage<T> &>(*this), static_cast<OptionalStorage<T> &>(other));
            } else {
                if (is_set == other.is_set) {
                    if (is_set) {
                        swap(data, other.data);
                    }
                } else {
                    auto &src = is_set ? *this : other;
                    auto &dst = is_set ? other : *this;
                    dst.construct_from(std::move(src.data));
                    src.reset();
                }
            }
        }

      protected:
        template <typename... A>
            requires std::constructible_from<T, A...>
        T &construct_from(A &&...args) noexcept(std::is_nothrow_constructible_v<T, A...>) {
            RETROLIB_ASSERT(!is_set);
            new (&data) T(std::forward<A>(args)...);
            is_set = true;
            return data;
        }

        template <typename I>
            requires std::constructible_from<T, I>
        T &construct_from_deref(const I &it) {
            RETROLIB_ASSERT(!is_set);
            new (&data) T(*it);
            is_set = true;
            return data;
        }

        template <typename U>
        constexpr void
        assign_from(U &&other) noexcept(std::is_nothrow_constructible_v<T, decltype(*std::forward<U>(other))> &&
                                        std::is_nothrow_assignable_v<T &, decltype(*std::forward<U>(other))>) {
            if (!other.has_value()) {
                reset();
            } else if (is_set) {
                data = std::forward<U>(other).data;
            } else {
                new (&data) T(*std::forward<U>(other));
                is_set = true;
            }
        }

      private:
        using OptionalStorage<T>::data;
        using OptionalStorage<T>::is_set;
    };

    /**
     * @brief A base class for managing an optional reference to an object.
     *
     * This class provides functionalities for handling an optional reference-type value,
     * including checking for its presence, accessing the reference, and managing its state.
     *
     * @tparam T The type of the referenced object.
     */
    template <ValidOptionalType T>
    struct OptionalBase<T &> {
        /**
         * @brief Default constructor for the OptionalBase class.
         *
         * Constructs an OptionalBase object with its default state.
         * This constructor is marked noexcept, indicating it does not throw exceptions.
         * It is also a defaulted constructor, which implies that the compiler will generate
         * the default functionality for the constructor.
         */
        OptionalBase() noexcept = default;

        /**
         * @brief Constructs an OptionalBase object that holds a reference to an object of type T.
         *
         * This constructor initializes the OptionalBase with a reference to an existing object
         * using the provided argument. It utilizes std::in_place_type_t to indicate in-place
         * construction of the reference type, ensuring type safety.
         *
         * @param arg An rvalue reference to an object of type A, which is used to initialize
         *            the internal reference of type T. The object must outlive the
         *            OptionalBase instance.
         * @return No return value as this is a constructor.
         */
        template <typename A>
            requires std::constructible_from<T &, A>
        constexpr explicit OptionalBase(std::in_place_type_t<T &>, A &&arg) noexcept : data(&arg) {
        }

        /**
         * @brief Checks whether the OptionalStorage contains a value.
         *
         * This method returns true if the OptionalStorage currently holds a value,
         * which is determined by checking if the internal data pointer is not null.
         * It is a constexpr and noexcept method, suitable for compile-time evaluation
         * and guaranteed not to throw exceptions.
         *
         * @return true if a value is present, false otherwise.
         */
        constexpr bool has_value() const noexcept {
            return data != nullptr;
        }

        /**
         * @brief Provides access to the contained object.
         *
         * This operator returns a reference to the object contained within,
         * asserting that the internal pointer is not null. The operation is
         * performed without throwing exceptions.
         *
         * @return A reference to the object of type T contained within.
         */
        constexpr T &operator*() noexcept {
            RETROLIB_ASSERT(data != nullptr);
            return *data;
        }

        /**
         * @brief Dereference operator to access the stored value.
         *
         * Returns a constant reference to the value stored in the object.
         * This operator is marked as constexpr and noexcept, ensuring
         * that it can be evaluated at compile time and does not throw exceptions.
         * An assertion verifies that the data pointer is not nullptr before dereferencing.
         *
         * @return A constant reference to the stored value.
         */
        constexpr const T &operator*() const noexcept {
            RETROLIB_ASSERT(data != nullptr);
            return *data;
        }

        /**
         * @brief Provides access to the underlying object of a pointer type.
         *
         * This operator function returns a pointer to the object held by the smart
         * pointer, ensuring that the pointer is not null before returning. It is
         * marked constexpr, allowing evaluation at compile time, and noexcept,
         * indicating it does not throw exceptions.
         *
         * @return A pointer to the underlying object.
         *
         * @pre The pointer `data` must not be nullptr.
         */
        constexpr T *operator->() noexcept {
            RETROLIB_ASSERT(data != nullptr);
            return data;
        }

        /**
         * @brief Provides access to the underlying data.
         *
         * This operator returns a pointer to the data of type T, allowing access
         * to its members. It asserts that the data is not a nullptr before returning.
         *
         * @return A constant pointer to the data of type T, ensuring no modifications.
         */
        constexpr const T *operator->() const noexcept {
            RETROLIB_ASSERT(data != nullptr);
            return data;
        }

        /**
         * @brief Resets the internal state of the object.
         *
         * This method sets the internal data pointer to nullptr, effectively
         * clearing any stored value or state. It is a constexpr and noexcept
         * function, allowing for compile-time evaluation and guaranteeing
         * that no exceptions will be thrown during its execution.
         */
        constexpr void reset() noexcept {
            data = nullptr;
        }

        /**
         * @brief Swaps the contents of this OptionalBase with another instance.
         *
         * This function swaps the data between the current instance and the provided
         * OptionalBase instance. If both instances contain non-null data, the data
         * is swapped directly. Otherwise, the pointers themselves are swapped.
         *
         * The function is conditionally noexcept, depending on whether the type T
         * used by OptionalBase is nothrow swappable.
         *
         * @param other Another instance of OptionalBase with which to swap the data.
         */
        constexpr void swap(OptionalBase &other) noexcept(std::is_nothrow_swappable_v<T>)
            requires std::swappable<T>
        {
            using std::swap;

            if (data != nullptr && other.data != nullptr) {
                swap(*data, *other.data);
            } else {
                swap(data, other.data);
            }
        }

      protected:
        template <typename U>
            requires std::convertible_to<U &, T &>
        constexpr T &construct_from(U &&ref) noexcept {
            RETROLIB_ASSERT(data == nullptr);
            data = &ref;
            return *data;
        }

        template <typename U>
        constexpr void assign_from(U &&other) {
            if (data != nullptr && other.data != nullptr) {
                *data = *std::forward<U>(other).data;
            } else {
                data = std::forward<U>(other).data;
            }
        }

      private:
        T *data = nullptr;
    };

    /**
     * Inheritance layer for an Optional of a type that has a non-trivial copy constructor.
     */
    template <ValidOptionalType T>
    struct OptionalCopy : OptionalBase<T> {

        /**
         * @brief Default constructor for the OptionalCopy class.
         *
         * This constructor initializes an OptionalCopy object that is in a valid state.
         * It is noexcept, indicating that it does not throw exceptions.
         */
        OptionalCopy() noexcept = default;

        /**
         * @brief Copy constructor for the OptionalCopy class.
         *
         * Constructs an OptionalCopy object by copying the state from another
         * OptionalCopy object. If the other object has a value, it copies that
         * value into the newly constructed object.
         *
         * @param other The OptionalCopy object to copy from.
         * @return A new instance of OptionalCopy with the same value state as the
         *         provided object, or in an empty state if the provided object
         *         is empty.
         *
         * @note This constructor is noexcept, provided the type T's copy
         *       constructor is noexcept.
         */
        OptionalCopy(const OptionalCopy &other) noexcept(std::is_nothrow_copy_constructible_v<T>) {
            if (other.has_value()) {
                this->construct_from(*other);
            }
        }

        /**
         * @brief Default move constructor for the OptionalCopy class.
         *
         * This constructor uses the default implementation provided by the compiler
         * to perform a member-wise move of the OptionalCopy object. It allows moving
         * resources from a temporary OptionalCopy object to a new instance, which can
         * improve performance by eliminating unnecessary copies.
         */
        OptionalCopy(OptionalCopy &&) = default;

        /**
         * @brief Default destructor for the OptionalCopy class.
         *
         * The default destructor provided by the compiler. This destructor will
         * automatically handle the destruction of any resources or members held
         * by the OptionalCopy object. It ensures proper cleanup without explicitly
         * defining a custom destructor.
         */
        ~OptionalCopy() = default;

        /**
         * @brief Default copy assignment operator for OptionalCopy.
         *
         * Provides the default functionality to assign one OptionalCopy object to another.
         * The default implementation performs a member-wise copy of all elements from the
         * source object to the destination object. It is a straightforward copy assignment,
         * ensuring that all member variables are duplicated exactly as they appear in the
         * source.
         */
        OptionalCopy &operator=(const OptionalCopy &) = default;

        /**
         * @brief Move assignment operator for the OptionalCopy class.
         *
         * Utilizes the default move assignment operator provided by the compiler.
         * Transfers the state from the specified rvalue object to this object,
         * leaving the source object in a valid but unspecified state.
         */
        OptionalCopy &operator=(OptionalCopy &&) = default;

        using OptionalBase<T>::OptionalBase;
    };

    /**
     * Layer of the Optional type depending on if it is copy constructable or not.
     */
    template <ValidOptionalType T>
    using CopyConstructLayer =
        std::conditional_t<std::is_copy_constructible_v<T> && !std::is_trivially_copy_constructible_v<T>,
                           OptionalCopy<T>, OptionalBase<T>>;

    /**
     * @brief Represents an optional wrapper with move semantics.
     *
     * The OptionalMove class extends the CopyConstructLayer class, providing a mechanism
     * to manage optional objects with move capabilities. It leverages move semantics to
     * optimize the transfer of resources. This class includes default constructor,
     * copy, move, assignment, and destructor operations.
     *
     * @tparam T The type of object managed by this optional wrapper.
     */
    template <ValidOptionalType T>
    struct OptionalMove : CopyConstructLayer<T> {
        /**
         * @brief Default constructor for the OptionalMove class.
         *
         * Initializes an OptionalMove object with its default state. This constructor
         * is marked as noexcept, indicating it is guaranteed not to throw exceptions.
         */
        OptionalMove() noexcept = default;

        /**
         * @brief Copy constructor for the OptionalMove class.
         *
         * Utilizes the default copy constructor to create a new OptionalMove object as
         * a copy of an existing one. This constructor will copy all members from the
         * source object to the newly constructed object.
         *
         * @param other The OptionalMove object to be copied.
         * @return A new OptionalMove object that is a copy of the provided object.
         */
        OptionalMove(const OptionalMove &) = default;

        /**
         * @brief Move constructor for the OptionalMove class.
         *
         * Constructs an OptionalMove object by transferring ownership of the managed object
         * from another OptionalMove instance. This constructor is conditional noexcept based
         * on the nothrow move constructibility of the managed type T. If the source object
         * has a value, it is moved to the new OptionalMove instance.
         *
         * @param other An rvalue reference to another OptionalMove instance to move from.
         */
        OptionalMove(OptionalMove &&other) noexcept(std::is_nothrow_move_constructible_v<T>) {
            if (other.has_value()) {
                this->construct_from(std::move(*other));
            }
        }

        /**
         * @brief Defaulted destructor for the OptionalMove class.
         *
         * Automatically generated by the compiler, this destructor is noexcept by default
         * and is responsible for cleaning up resources when an OptionalMove object goes
         * out of scope. The default implementation is sufficient as it relies on the
         * destruction of member variables and handles resource management correctly
         * without additional logic.
         */
        ~OptionalMove() = default;

        /**
         * @brief Copy assignment operator for the OptionalMove class.
         *
         * Uses the default compiler-generated implementation to perform
         * a copy assignment of the current OptionalMove object from another.
         *
         * @param other The OptionalMove object to copy from.
         * @return A reference to the current OptionalMove object after assignment.
         */
        OptionalMove &operator=(const OptionalMove &) = default;

        /**
         * @brief Move assignment operator for the OptionalMove class.
         *
         * This operator is used to assign the state of one OptionalMove object to
         * another via move semantics. It is marked as default, allowing the compiler
         * to automatically generate the implementation.
         *
         * @param other The OptionalMove object to be moved.
         * @return A reference to the modified OptionalMove object (*this).
         */
        OptionalMove &operator=(OptionalMove &&) = default;

        using CopyConstructLayer<T>::CopyConstructLayer;
    };

    /**
     * Layer of the Optional type depending on if we need custom move construction..
     */
    template <ValidOptionalType T>
    using MoveConstructLayer =
        std::conditional_t<std::is_move_constructible_v<T> && !std::is_trivially_move_constructible_v<T>,
                           OptionalMove<T>, CopyConstructLayer<T>>;

    /**
     * @brief Struct for OptionalCopyAssign which extends MoveConstructLayer.
     *
     * This struct provides several default operations including default constructor,
     * copy constructor, move constructor, and destructor. It also includes a copy
     * assignment operator with noexcept specifier, leveraging the noexcept property
     * of the underlying type T. The copy assignment operator assigns from another
     * instance of OptionalCopyAssign.
     *
     * @tparam T Type parameter for the MoveConstructLayer.
     */
    template <ValidOptionalType T>
    struct OptionalCopyAssign : MoveConstructLayer<T> {
        /**
         * @brief Default copy assignment operator for the OptionalCopyAssign class.
         *
         * Implements the default behavior for copy assignment, which assigns the
         * values from another OptionalCopyAssign object to this one, if present.
         * This operation is marked as noexcept, indicating it is guaranteed not to
         * throw exceptions.
         *
         * @return A reference to the current instance after assignment.
         */
        OptionalCopyAssign() noexcept = default;

        /**
         * @brief Copy constructor for the OptionalCopyAssign class.
         *
         * This defaulted copy constructor enables the creation of a new
         * OptionalCopyAssign object as a copy of an existing one, using the
         * default copy behavior provided by the compiler.
         */
        OptionalCopyAssign(const OptionalCopyAssign &) = default;

        /**
         * @brief Default move constructor for the OptionalCopyAssign class.
         *
         * This constructor is automatically generated by the compiler and allows an
         * OptionalCopyAssign object to be constructed from an rvalue reference of another
         * OptionalCopyAssign object. It is a default operation ensuring efficient transfer
         * of resources from the source object without copying. This operation is noexcept
         * if all member variables are noexcept movable.
         *
         * @param other An rvalue reference to the OptionalCopyAssign object to be moved.
         * @return A new OptionalCopyAssign object with the resources of the original object.
         */
        OptionalCopyAssign(OptionalCopyAssign &&) = default;

        /**
         * @brief Default destructor for the OptionalCopyAssign class.
         *
         * This destructor ensures that any resources acquired by an
         * instance of OptionalCopyAssign are properly released when
         * the instance is destructed. The use of `= default` indicates that
         * the compiler will generate the default implementation of this
         * destructor, which is guaranteed to be constexpr and noexcept.
         */
        ~OptionalCopyAssign() = default;

        /**
         * @brief Copy assignment operator for the OptionalCopyAssign class.
         *
         * Assigns the value from another OptionalCopyAssign object to this one.
         * The operation is noexcept if the copy assignment and construction
         * operations for type T are noexcept. Uses the assign_from method for
         * the actual copy process.
         *
         * @param other The OptionalCopyAssign object to copy from.
         * @return A reference to this OptionalCopyAssign object after the assignment.
         */
        OptionalCopyAssign &
        operator=(const OptionalCopyAssign &other) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                            std::is_nothrow_constructible_v<T, decltype(*other)>) {
            this->assign_from(other);
            return *this;
        }

        /**
         * @brief Move assignment operator for the OptionalCopyAssign class.
         *
         * Assigns the contents of another OptionalCopyAssign object using move semantics.
         * This move assignment operator is defined as default, meaning the compiler will
         * generate the default implementation of moving the data from the right-hand side
         * object to the left-hand side object. The default implementation performs a member-wise
         * move of all elements from the source object to this object.
         *
         * @return A reference to *this with the newly moved contents.
         */
        OptionalCopyAssign &operator=(OptionalCopyAssign &&) = default;

        using MoveConstructLayer<T>::MoveConstructLayer;
    };

    /**
     * Optional layer for when the copy assign operation is deleted.
     */
    template <ValidOptionalType T>
    struct DeletedCopyAssign : MoveConstructLayer<T> {
        /**
         * @brief Default constructor for the DeletedCopyAssign class.
         *
         * Constructs a DeletedCopyAssign object using the default constructor. It is a
         * noexcept operation, ensuring no exceptions are thrown during the construction.
         */
        DeletedCopyAssign() noexcept = default;

        /**
         * @brief Copy constructor for the DeletedCopyAssign class.
         *
         * This is the default copy constructor for the DeletedCopyAssign class.
         * The implementation is compiler-generated and does not perform any
         * additional actions beyond copying the contents of the source object.
         *
         * @param other The DeletedCopyAssign object to be copied.
         */
        DeletedCopyAssign(const DeletedCopyAssign &) = default;

        /**
         * @brief Move constructor for the DeletedCopyAssign class.
         *
         * Initializes a DeletedCopyAssign object by transferring the resources from
         * the specified rvalue object. This constructor is set to default, allowing
         * the compiler to generate the move operation automatically.
         *
         * @param other The rvalue reference to another DeletedCopyAssign object
         * from which to move the resources.
         * @return A new instance of DeletedCopyAssign initialized via move
         * semantics.
         */
        DeletedCopyAssign(DeletedCopyAssign &&) = default;

        /**
         * @brief Destructor for the DeletedCopyAssign class.
         *
         * This is the defaulted destructor for the DeletedCopyAssign class. It
         * indicates that resources managed by instances of this class are properly
         * cleaned up when they go out of scope, following the rule of zero where
         * the compiler-generated destructor is sufficient to manage cleanup.
         */
        ~DeletedCopyAssign() = default;

        /**
         * @brief Deleted copy assignment operator.
         *
         * This operator is deleted to prevent copying of objects of the class
         * it is a member of. This ensures that the class is non-copy-assignable.
         *
         * @param other The object to copy from. This parameter is unused as the
         * operation is deleted.
         * @return DeletedCopyAssign& This return type is simply a part of the
         * function signature and is not applicable as the operation is deleted.
         */
        DeletedCopyAssign &operator=(const DeletedCopyAssign &) = delete;

        /**
         * @brief Move assignment operator for the DeletedCopyAssign class.
         *
         * Implements the default move assignment operation for DeletedCopyAssign objects.
         * This operator will allow a DeletedCopyAssign object to be moved from one
         * instance to another safely, ensuring that resources are transferred without duplication.
         *
         * @param other The DeletedCopyAssign instance from which to move data.
         * @return A reference to the current instance with the moved data.
         */
        DeletedCopyAssign &operator=(DeletedCopyAssign &&) = default;

        using MoveConstructLayer<T>::MoveConstructLayer;
    };

    /**
     * Optional layer for the copy assignment operation.
     */
    template <ValidOptionalType T>
    using CopyAssignLayer =
        std::conditional_t<std::is_copy_constructible_v<T> && std::is_copy_assignable_v<T>,
                           std::conditional_t<std::is_reference_v<T> || !(std::is_trivially_copy_constructible_v<T> &&
                                                                          std::is_trivially_copy_assignable_v<T>),
                                              OptionalCopyAssign<T>, MoveConstructLayer<T>>,
                           DeletedCopyAssign<T>>;

    /**
     * @brief Default constructor for the OptionalMoveAssign struct.
     *
     * This constructor initializes an instance of OptionalMoveAssign
     * with a default-constructed object maintaining an empty state.
     * This constructor is noexcept, ensuring it does not throw exceptions.
     */
    template <ValidOptionalType T>
    struct OptionalMoveAssign : CopyAssignLayer<T> {
        /**
         * @brief Default move assignment operator for the OptionalMoveAssign class.
         *
         * The move assignment operator allows for the transfer of resources from another
         * OptionalMoveAssign object to this one. It is defined as noexcept, ensuring that
         * it does not throw exceptions during the operation.
         *
         * @return A reference to the assigned OptionalMoveAssign object.
         */
        OptionalMoveAssign() noexcept = default;

        /**
         * @brief Default move assignment operator for the OptionalMoveAssign class.
         *
         * Performs a default move assignment, relying on the compiler-generated
         * implementation. It ensures that all member variables are moved from
         * the source object to the destination object.
         *
         * @param other The source OptionalMoveAssign object to be moved.
         * @return A reference to the current object after the move assignment.
         */
        OptionalMoveAssign(const OptionalMoveAssign &) = default;

        /**
         * @brief Move assignment operator for OptionalMoveAssign class.
         *
         * Performs a move assignment from another OptionalMoveAssign object. This
         * operator uses the compiler-generated default implementation, which
         * efficiently transfers the resources from the given object to this
         * object, leaving the given object in a valid but unspecified state.
         *
         * @param other An rvalue reference to an OptionalMoveAssign object that
         * is the source of the move. After the operation, this object will be
         * assigned the resources of the source object.
         * @return A reference to this OptionalMoveAssign object after assignment.
         */
        OptionalMoveAssign(OptionalMoveAssign &&other) = default;

        /**
         * @brief Default destructor for the OptionalMoveAssign class.
         *
         * Automatically generates a destructor that properly cleans up the resources
         * used by the OptionalMoveAssign object. It is defined as default, allowing
         * the compiler to handle the destruction process.
         */
        ~OptionalMoveAssign() = default;

        /**
         * @brief Default copy assignment operator for the OptionalMoveAssign class.
         *
         * Performs a copy assignment of an OptionalMoveAssign object from another
         * OptionalMoveAssign object. The operation is implicitly defined by the
         * compiler, ensuring member-wise copy semantics.
         */
        OptionalMoveAssign &operator=(const OptionalMoveAssign &other) = default;

        /**
         * @brief Move assignment operator for the OptionalMoveAssign class.
         *
         * Transfers ownership of the resource from another OptionalMoveAssign object.
         * This operator is noexcept if both the move constructor and move assignment
         * for the contained type T are noexcept.
         *
         * @param other An rvalue reference to another OptionalMoveAssign object.
         * @return A reference to the current instance after assignment.
         */
        OptionalMoveAssign &operator=(OptionalMoveAssign &&other) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                                           std::is_nothrow_move_assignable_v<T>) {
            this->assign_from(std::move(other));
            return *this;
        }

        using CopyAssignLayer<T>::CopyAssignLayer;
    };

    /**
     * Layer for when the move assignment is deleted.
     */
    template <ValidOptionalType T>
    struct DeletedMoveAssign : CopyAssignLayer<T> {
        /**
         * @brief Defaulted deleted move assignment operator.
         *
         * This move assignment operator is defaulted and deleted, meaning it is automatically
         * generated by the compiler as a deleted function. It indicates that move assignment
         * is intentionally disallowed for objects of this type. The noexcept specifier declares
         * that this operation does not throw exceptions.
         *
         * @return Deleted function, does not return a value.
         */
        DeletedMoveAssign() noexcept = default;

        /**
         * @brief Copy constructor for DeletedMoveAssign class.
         *
         * The DeletedMoveAssign copy constructor is explicitly defaulted,
         * allowing for a member-wise copy of the object. This constructor is implicitly
         * noexcept and enables the class to be copy-constructed using default behavior.
         */
        DeletedMoveAssign(const DeletedMoveAssign &) = default;

        /**
         * @brief Default move constructor for DeletedMoveAssign class.
         *
         * Initializes a DeletedMoveAssign object by moving the resources from
         * another object of the same class. This move constructor is defaulted,
         * meaning it will utilize the compiler-generated implementation.
         */
        DeletedMoveAssign(DeletedMoveAssign &&) = default;

        /**
         * @brief Defaulted destructor for move assignment.
         *
         * This is a default move assignment operator. It is automatically generated
         * by the compiler to manage resources for the class when the object is
         * assigned from another using move semantics.
         */
        ~DeletedMoveAssign() = default;

        /**
         * @brief Copy assignment operator for DeletedMoveAssign class.
         *
         * This operator is defaulted and allows for the assignment of one
         * DeletedMoveAssign object to another using the copy semantics.
         */
        DeletedMoveAssign &operator=(const DeletedMoveAssign &) = default;

        /**
         * @brief Deleted move assignment operator for the DeletedMoveAssign class.
         *
         * This operator is explicitly deleted to prevent move assignment operations
         * on instances of the DeletedMoveAssign class. Attempting to use move
         * assignment on an object of this class will result in a compilation error.
         */
        DeletedMoveAssign &operator=(DeletedMoveAssign &&) = delete;

        using CopyAssignLayer<T>::CopyAssignLayer;
    };

    /**
     * Layer for when the move assignment is deleted.
     */
    template <ValidOptionalType T>
    using MoveAssignLayer =
        std::conditional_t<std::is_move_constructible_v<T> && std::is_move_assignable_v<T>,
                           std::conditional_t<std::is_reference_v<T> || !(std::is_trivially_move_constructible_v<T> &&
                                                                          std::is_trivially_move_assignable_v<T>),
                                              OptionalMoveAssign<T>, CopyAssignLayer<T>>,
                           DeletedMoveAssign<T>>;

    /**
     * Concept to check if we can convert between various optional types.
     */
    template <typename U, typename T>
    concept OptionalShouldConvert =
        !(std::constructible_from<T, Optional<U> &> || std::constructible_from<T, Optional<U> &&> ||
          std::constructible_from<T, const Optional<U> &> || std::constructible_from<T, const Optional<U> &&> ||
          std::convertible_to<Optional<U> &, T> || std::convertible_to<Optional<U> &&, T> ||
          std::convertible_to<const Optional<U> &, T> || std::convertible_to<const Optional<U> &&, T>);

    /**
     * Concept to check if we can assign between various optional types.
     */
    template <typename U, typename T>
    concept OptionalShouldConvertAssign =
        OptionalShouldConvert<U, T> ||
        !(std::assignable_from<T &, Optional<U> &> || std::assignable_from<T &, Optional<U> &&> ||
          std::assignable_from<T &, const Optional<U> &> || std::assignable_from<T &, const Optional<U> &&>);

    /**
     * @brief Class template for managing optional values with enhanced assignment and construction capabilities.
     *
     * This class template, `Optional`, extends basic optional functionality by allowing
     * more flexible construction and assignment operations. The class template is
     * designed to handle both lvalue and rvalue references correctly and supports
     * conversion between different `Optional` types.
     *
     * @tparam T The type of the value managed by the `Optional` instance.
     */
    template <ValidOptionalType T>
    class Optional : private MoveAssignLayer<T> {
        using Base = MoveAssignLayer<T>;

      public:
        using ValueType = std::conditional_t<std::is_lvalue_reference_v<T>, T, std::remove_cv_t<T>>;

        /**
         * @brief Default constructor for the Optional class.
         *
         * Constructs an Optional object with a default state. As a constexpr and noexcept
         * constructor, it can be used in compile-time expressions and guarantees not to
         * throw exceptions.
         */
        constexpr Optional() noexcept = default;

        /**
         * @brief Constructs an Optional object with an empty state.
         *
         * This constructor creates an Optional object that is initialized to an empty
         * state by accepting a parameter of std::nullopt_t type. It is a constexpr
         * constructor allowing evaluation at compile time and is marked noexcept to
         * indicate it will not throw exceptions during its execution.
         */
        constexpr explicit(false) Optional(std::nullopt_t) noexcept : Base() {
        }

        /**
         * @brief Constructs an Optional object from a given value.
         *
         * This constructor accepts a value of type U and initializes the Optional object with the provided value.
         * It uses a constrained explicit keyword that depends on the convertibility of type U to type T.
         * The constructor utilizes std::in_place_type to construct the value in place, and it forwards the
         * value to ensure perfect forwarding semantics.
         *
         * @tparam U The type of the value being used to initialize the Optional object.
         * @param value The value used to initialize the Optional object.
         */
        template <ValidOptionalType U = T>
            requires std::constructible_from<T, U> && (!std::same_as<std::decay_t<U>, Optional>)
        constexpr explicit(!std::convertible_to<U, T>) Optional(U &&value)
            : Base(std::in_place_type<T>, std::forward<U>(value)) {
        }

        /**
         * @brief Copy constructor for the Optional class.
         *
         * This defaulted copy constructor allows copying an existing Optional object,
         * creating a new Optional with the same state and value as the original.
         * It relies on the compiler-generated copy constructor.
         *
         * @param other The Optional object to be copied.
         * @return A new Optional instance with the same state and value as the original.
         */
        Optional(const Optional &) = default;

        /**
         * @brief Move constructor for the Optional class.
         *
         * This default move constructor allows an Optional object to be constructed
         * by transferring the contents from another Optional object. It enables
         * efficient transfer of resources and is automatically noexcept if the
         * move constructor of the stored type is noexcept.
         *
         * @param other The Optional object to be moved from.
         * @return A new Optional object containing the contents of the moved-from object.
         */
        Optional(Optional &&) = default;

        using Base::Base;

        /**
         * @brief Constructs an Optional object from another Optional of a different type.
         *
         * This constructor allows for the creation of an Optional object from another
         * Optional<U> object. It is constexpr, meaning it can be evaluated at compile time.
         * The use of explicit(!std::convertible_to<const U &, T>) enforces that the conversion
         * is explicit unless U can be implicitly converted to T. If the `other` Optional
         * has a value, `construct_from` is called to initialize the current Optional with
         * the value contained in `other`.
         *
         * @tparam U The type of the value contained in the `other` Optional object.
         * @param other The Optional<U> object from which to construct the new Optional.
         *              The constructed object will contain a value only if `other` has a value.
         */
        template <OptionalShouldConvert<T> U>
            requires std::constructible_from<T, const U &> && (!std::same_as<std::decay_t<U>, T>)
        constexpr explicit(!std::convertible_to<const U &, T>) Optional(const Optional<U> &other) {
            if (other.has_value()) {
                Base::construct_from(*other);
            }
        }

        /**
         * @brief Move constructor for the Optional class template.
         *
         * Constructs an Optional object by transferring the value from another Optional instance
         * of a potentially different type U. The constructor is constrained to ensure that an
         * implicit conversion is only allowed if the type U is convertible to T, which is
         * enforced with the compile-time requirement.
         *
         * @param other An rvalue reference to an Optional<U> object from which to move the value.
         *              If the other object contains a value, it is moved into this Optional.
         *              Otherwise, this Optional remains empty.
         * @return An Optional<T> object initialized with the moved value from the other Optional
         *         object, if present. Otherwise, it remains in an empty state.
         */
        template <OptionalShouldConvert<T> U>
            requires std::constructible_from<T, U &&> && (!std::same_as<std::decay_t<U>, T>)
        constexpr explicit(!std::convertible_to<const U &, T>) Optional(Optional<U> &&other) {
            if (other.has_value()) {
                Base::construct_from(std::move(*other));
            }
        }

        /**
         * @brief Defaulted destructor for the Optional class.
         *
         * This destructor is defaulted, indicating that the compiler should generate
         * the default implementation, which will appropriately handle destruction
         * of the contained object if it exists, ensuring proper resource management.
         */
        ~Optional() = default;

        /**
         * @brief Assigns a null optional state to the Optional object.
         *
         * This constexpr and noexcept assignment operator sets the Optional object
         * to an empty state using std::nullopt. Calling this operator will reset
         * any current value held by the Optional object, rendering it empty.
         *
         * @return Reference to the modified Optional object, now in an empty state.
         */
        constexpr Optional &operator=(std::nullopt_t) noexcept {
            reset();
            return *this;
        }

        /**
         * @brief Assignment operator for the Optional class.
         *
         * Performs a copy assignment from another Optional object. This operator is
         * constexpr and noexcept, which ensures that it can be evaluated at compile time and
         * does not throw exceptions. The default implementation is used, providing standard
         * copy assignment behavior.
         *
         * @param other The Optional object to be copied.
         * @return A reference to the assigned Optional object.
         */
        constexpr Optional &operator=(const Optional &other) = default;

        /**
         * @brief Assignment operator for move semantics.
         *
         * Assigns the state of another Optional object to this Optional object
         * using move semantics. This operator is defaulted, indicating that the
         * compiler generates the implementation.
         *
         * @param other The Optional object to be moved from.
         * @return A reference to this Optional object with the moved state.
         */
        constexpr Optional &operator=(Optional &&other) = default;

        /**
         * @brief Assignment operator for the Optional class.
         *
         * This operator assigns a value to an Optional object. If the Optional already holds a value,
         * it uses the assignment operation; otherwise, it constructs a new value from the given input.
         * The operation noexcept specification depends on the ability to construct and assign the type T
         * with the provided type U without throwing exceptions.
         *
         * @param other The value to be assigned to the Optional object, either by constructing or
         * assigning based on the current state of the Optional.
         * @return A reference to the current Optional instance after the assignment.
         */
        template <ValidOptionalType U = T>
            requires(!std::same_as<std::decay_t<U>, Optional> && std::constructible_from<T, U> &&
                     std::assignable_from<T &, U>)
        constexpr Optional &operator=(U &&other) noexcept(std::is_nothrow_constructible_v<T, U> &&
                                                          std::is_nothrow_assignable_v<T &, U>) {
            if (has_value()) {
                **this = std::forward<U>(other);
            } else {
                Base::construct_from(std::forward<U>(other));
            }

            return *this;
        }

        /**
         * @brief Copy assignment operator for the Optional class.
         *
         * Assigns the value from another Optional object of potentially different type U
         * to this Optional object. This utilizes the assign_from method of the Base class
         * to perform the assignment. The operator returns a reference to the modified
         * Optional object.
         *
         * @tparam U The type of the value contained within the other Optional object.
         * @param other The other Optional object of type U to assign from.
         * @return A reference to the updated Optional object.
         */
        template <OptionalShouldConvertAssign<T> U>
            requires std::constructible_from<T, const U &> && std::assignable_from<T &, const U &> &&
                     (!std::same_as<std::decay_t<U>, T>)
        constexpr Optional &operator=(const Optional<U> &other) {
            Base::assign_from(other);
            return *this;
        }

        /**
         * @brief Move assignment operator for the Optional class template.
         *
         * Assigns the state and the value from another Optional object of a potentially
         * different type U to the current Optional object. Utilizes the move semantics
         * by transferring ownership of the resource from the right-hand side object,
         * minimizing copying and improving performance.
         *
         * This function template ensures that the assignment can be done for
         * optionals holding different types, provided the assignment is valid
         * between the contained types.
         *
         * @tparam U The type contained in the other Optional object.
         * @param other The Optional object to move-assign from.
         * @return A reference to the current Optional object with updated contents.
         */
        template <OptionalShouldConvertAssign<T> U>
            requires std::constructible_from<T, U &&> && std::assignable_from<T &, U &&> &&
                     (!std::same_as<std::decay_t<U>, T>)
        constexpr Optional &operator=(Optional<U> &&other) {
            Base::assign_from(std::move(other));
            return *this;
        }

        /**
         * @brief Reinitializes and constructs an object from a dereferenced iterator.
         *
         * The method first resets the current state and then constructs a new object by
         * dereferencing the provided iterator. It returns a reference to the newly constructed object.
         *
         * @tparam I The type of the iterator used for dereferencing.
         * @param it The iterator whose dereferenced value will be used for object construction.
         * @return A reference to the newly constructed object.
         */
        template <typename I>
            requires std::constructible_from<T, decltype(*std::declval<const I &>())>
        T &emplace_deref(const I &it) {
            reset();
            return Base::construct_from_deref(it);
        }

        /**
         * @brief Constructs an object of type T in-place using the provided arguments.
         *
         * The emplace method resets any existing state and constructs a new object
         * of type T with the given arguments. It ensures that the construction is
         * noexcept if the type T is noexcept constructible with the specified arguments.
         *
         * @param args The arguments to be forwarded to the constructor of T.
         *
         * @return A reference to the newly constructed object of type T.
         */
        template <typename... A>
            requires std::constructible_from<T, A...>
        T &emplace(A &&...args) noexcept(std::is_nothrow_constructible_v<T, A...>) {
            reset();
            return Base::construct_from(std::forward<A>(args)...);
        }

        /**
         * @brief Constructs and initializes an object of type T in the storage space.
         *
         * Reinitializes the contained object with the provided initializer list and additional arguments.
         * This method is noexcept if T can be constructed without throwing exceptions from the given
         * initializer list and forwarded arguments.
         *
         * @param initializer_list An initializer list of elements of type U used to initialize an object of type T.
         * @param args Additional arguments used to further initialize the object of type T.
         * @return A reference to the newly constructed object of type T.
         */
        template <typename U, typename... A>
            requires std::constructible_from<T, std::initializer_list<U>, A...>
        T &emplace(std::initializer_list<U> initializer_list,
                   A &&...args) noexcept(std::is_nothrow_constructible_v<T, std::initializer_list<U>, A...>) {
            reset();
            return Base::construct_from(initializer_list, std::forward<A>(args)...);
        }

        using Base::swap;

        /**
         * @brief Swaps the contents of two Optional objects.
         *
         * This constexpr friend function allows swapping the values contained in two
         * Optional instances, `lhs` and `rhs`. It is marked noexcept, and its exception
         * specification depends on the noexcept status of the internal swap operation.
         *
         * @param lhs The first Optional object to swap.
         * @param rhs The second Optional object to swap.
         */
        constexpr friend void swap(Optional &lhs, Optional &rhs) noexcept(noexcept(lhs.swap(rhs))) {
            lhs.swap(rhs);
        }

        using Base::operator->;
        using Base::operator*;

        using Base::has_value;

        /**
         * @brief Provides access to the stored value.
         *
         * This method returns a reference to the contained value if an Optional value
         * is present. If no value is present, it throws a `bad_optional_access` exception.
         *
         * It should be noted that this function is a constexpr and can be evaluated
         * at compile time if invoked within a constexpr context, and the Optional is
         * in a valid state. This overload will be used when the Optional object is
         * an lvalue.
         *
         * @return Reference to the stored value if present.
         * @throws std::bad_optional_access if no value is present.
         */
        constexpr T &value() & {
            return has_value() || throw_bad_optional_access(), **this;
        }

        /**
         * @brief Returns a constant reference to the contained value.
         *
         * Provides access to the value stored in the optional object. If the optional
         * contains a value, it returns a constant reference to it. If the optional
         * does not contain a value, it throws an exception of type std::bad_optional_access.
         * The function is marked as constexpr, allowing it to be evaluated at compile time,
         * and noexcept, indicating it does not throw exceptions during the execution of the
         * has_value check.
         *
         * @return A constant reference to the contained value.
         * @throws std::bad_optional_access if the optional does not contain a value.
         */
        constexpr const T &value() const & {
            return has_value() || throw_bad_optional_access(), **this;
        }

        /**
         * @brief Retrieves the stored value with move semantics from a temporary optional object.
         *
         * This function returns the contained value of an optional object, utilizing move semantics
         * for efficiency. It checks whether the optional object has a value. If the object holds
         * no value, it throws an exception of type bad_optional_access. The function is marked
         * as constexpr, indicating that it can be evaluated at compile time, and applies only to
         * rvalue contexts, ensuring the optional object is a temporary that can be safely moved.
         *
         * @return T&& The contained value, moved from the optional object.
         *
         * @throws std::bad_optional_access if the optional object does not contain a value.
         */
        constexpr T &&value() && {
            return has_value() || throw_bad_optional_access(), std::move(**this);
        }

        /**
         * @brief Retrieves the value of the optional object, returning an rvalue reference.
         *
         * This function attempts to return the contained value as an rvalue reference when
         * the optional object has a value. If the optional object does not have a value,
         * it throws an exception of type bad_optional_access. The function is marked constexpr
         * meaning it can be used in constant expressions, and it is also marked noexcept
         * to indicate no exception will be thrown under normal conditions.
         *
         * @return A const rvalue reference to the contained value of type T if the
         * optional object has a value. If not, it throws an exception.
         *
         * @throws bad_optional_access if there is no value present in the optional object.
         */
        constexpr const T &&value() const && {
            return has_value() || throw_bad_optional_access(), std::move(**this);
        }

        template <ValidOptionalType U>
            requires std::copy_constructible<T> && std::convertible_to<U, T>
        /**
         * @brief Returns the stored value or a provided default value.
         *
         * Retrieves the stored value if the optional has a value; otherwise, returns
         * the provided default value. This function is declared as `constexpr` and can
         * be evaluated at compile time. It uses reference semantics, and the default
         * value is forwarded using perfect forwarding to efficiently handle lvalues
         * and rvalues.
         *
         * @tparam U Type of the provided default value.
         * @param default_value The value to be returned if the optional does not have a value.
         * @return The stored value if the optional has one; otherwise, the provided
         * default value.
         */
        constexpr T value_or(U &&default_value) const & {
            return has_value() ? **this : std::forward<U>(default_value);
        }

        template <ValidOptionalType U>
            requires std::move_constructible<U> && std::convertible_to<U, T>
        /**
         * @brief Returns the contained value or a provided default.
         *
         * This method returns the contained value if it exists, and uses std::move to
         * ensure efficient transfer of the value. If the optional does not contain a
         * value, it returns the provided default_value, forwarding it appropriately
         * using std::forward. This function is marked as a constexpr, allowing for
         * compile-time evaluation, and is a rvalue-qualified member function.
         *
         * @tparam U Type of the default_value parameter.
         * @param default_value The value to return if the optional is empty.
         * @return Either the contained value or default_value.
         */
        constexpr T value_or(U &&default_value) && {
            return has_value() ? std::move(**this) : std::forward<U>(default_value);
        }

        using Base::reset;

        /**
         * @brief Returns an iterator to the beginning of the data structure.
         *
         * This method initializes and returns an OptionalIterator that points to
         * the beginning of the container. The returned iterator allows traversal
         * of the elements contained within the OptionalStorage.
         *
         * @return An OptionalIterator pointing to the beginning of the container.
         */
        constexpr auto begin() {
            return optionals::OptionalIterator(*this);
        }

        /**
         * @brief Returns a constant iterator to the beginning of the optional storage.
         *
         * This method constructs and returns an `OptionalIterator` pointing to the
         * beginning of the optional storage. The returned iterator is constant
         * (read-only) and operates on the current instance.
         *
         * @return A constant `OptionalIterator` to the beginning of the optional storage.
         */
        constexpr auto begin() const {
            return optionals::OptionalIterator(*this);
        }

        /**
         * @brief Provides the end sentinel for an optional-based iteration.
         *
         * This function returns an OptionalSentinel object that represents the end of
         * a sequence in optional-based iteration. It is a constexpr function, allowing
         * compile-time evaluation, and is marked as const to indicate that it does not
         * modify the state of the object.
         *
         * @return An OptionalSentinel object indicating the end of the sequence.
         */
        constexpr optionals::OptionalSentinel end() const {
            return optionals::OptionalSentinel();
        }

        /**
         * @brief Determines the size based on the presence of a value.
         *
         * Returns 1 if a value is present, otherwise returns 0. This method is
         * constexpr, allowing it to be evaluated at compile time.
         *
         * @return The size as 1 if a value exists, or 0 if no value is present.
         */
        constexpr size_t size() const {
            return has_value() ? 1 : 0;
        }
    };

    /**
     * @brief Equality comparison operator for Optional objects.
     *
     * Compares two Optional objects for equality. Returns true if both Optionals
     * have the same value state (either both have values or both do not have values)
     * and, if both have values, their contained values are equal.
     *
     * @tparam T The type of the value contained in the first Optional object.
     * @tparam U The type of the value contained in the second Optional object.
     * @param self The first Optional object to compare.
     * @param other The second Optional object to compare.
     * @return constexpr bool True if both Optionals have the same value state and
     * contain equal values; false otherwise.
     *
     * @note The function is noexcept and can be evaluated at compile time if the
     * comparison of the contained values is noexcept.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, ValidOptionalType U>
    constexpr bool operator==(const Optional<T> &self, const Optional<U> &other) noexcept(noexcept(*self == *other)) {
        return self.has_value() == other.has_value() && (!self.has_value() || *self == *other);
    }

    /**
     * @brief Three-way comparison operator for Optional types.
     *
     * Compares two Optional objects of potentially different types using the
     * three-way comparison operator. If one Optional has a value and the other
     * does not, it returns std::compare_three_way_result_t::greater or
     * std::compare_three_way_result_t::less accordingly. If neither has a value,
     * it returns std::compare_three_way_result_t::equivalent. If both have values,
     * it performs a comparison between the contained values.
     *
     * @tparam T The type of the value contained in the first Optional.
     * @tparam U The type of the value contained in the second Optional.
     * @param self The first Optional to compare.
     * @param other The second Optional to compare.
     * @return Result of the three-way comparison.
     * @note This operator is constexpr and noexcept, provided that the comparison
     *       of the contained types is noexcept.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, ValidOptionalType U>
    constexpr auto operator<=>(const Optional<T> &self, const Optional<U> &other) noexcept(noexcept(*self <=> *other)) {
        using ResultType = std::compare_three_way_result_t<T, U>;
        if (self.has_value() && !other.has_value()) {
            return ResultType::greater;
        }

        if (!self.has_value() && other.has_value()) {
            return ResultType::less;
        }

        if (!self.has_value() && !other.has_value()) {
            return ResultType::equivalent;
        }

        return *self <=> *other;
    }

    /**
     * @brief Equality operator for comparing an Optional object with std::nullopt.
     *
     * This constexpr and noexcept function checks if the Optional object is in an
     * empty state by comparing it with std::nullopt. It returns true if the Optional
     * object does not contain a value, otherwise it returns false.
     *
     * @tparam T The type of the value that may be contained in the Optional object.
     * @param self The Optional object to be compared.
     * @param std::nullopt_t A tag indicating an empty optional state.
     * @return true if the Optional object is empty, false otherwise.
     */
    RETROLIB_EXPORT template <ValidOptionalType T>
    constexpr bool operator==(const Optional<T> &self, std::nullopt_t) noexcept {
        return !self.has_value();
    }

    /**
     * @brief Compares an Optional object to std::nullopt.
     *
     * This operator performs a comparison between an Optional object and std::nullopt.
     * If the Optional object contains a value, it returns std::strong_ordering::greater.
     * If the Optional object does not contain a value, it returns std::strong_ordering::equal.
     *
     * @tparam T The type of the value stored in the Optional.
     * @param self The Optional object being compared.
     * @return std::strong_ordering::greater if the Optional has a value, std::strong_ordering::equal otherwise.
     *
     * @note This function is marked as constexpr, allowing it to be evaluated at compile time, and noexcept, indicating
     * that it does not throw exceptions.
     */
    RETROLIB_EXPORT template <ValidOptionalType T>
    constexpr std::strong_ordering operator<=>(const Optional<T> &self, std::nullopt_t) noexcept {
        return self.has_value() ? std::strong_ordering::greater : std::strong_ordering::equal;
    }

    /**
     * @brief Equality comparison operator for std::nullopt_t and Optional.
     *
     * Compares a std::nullopt_t with an Optional object to determine equality.
     * The operation checks if the Optional object does not contain a value.
     * It is a constexpr and noexcept function, allowing for compile-time
     * evaluation and ensuring no exceptions are thrown during execution.
     *
     * @tparam T Type of the value contained within the Optional object.
     * @param other The Optional object to compare against std::nullopt_t.
     * @return True if the Optional object does not have a value, false otherwise.
     */
    RETROLIB_EXPORT template <ValidOptionalType T>
    constexpr bool operator==(std::nullopt_t, const Optional<T> &other) noexcept {
        return !other.has_value();
    }

    /**
     * @brief Three-way comparison operator for std::nullopt_t and Optional<T>.
     *
     * This constexpr, noexcept function compares an instance of std::nullopt_t with an Optional<T>
     * object. It returns std::strong_ordering::less if the Optional<T> object has a value, and
     * std::strong_ordering::equal if it does not have a value.
     *
     * @tparam T The type contained by the Optional object being compared.
     * @param other The Optional object being compared with std::nullopt_t.
     * @return std::strong_ordering::less if the Optional has a value, otherwise std::strong_ordering::equal.
     */
    RETROLIB_EXPORT template <ValidOptionalType T>
    constexpr std::strong_ordering operator<=>(std::nullopt_t, const Optional<T> &other) noexcept {
        return other.has_value() ? std::strong_ordering::less : std::strong_ordering::equal;
    }

    /**
     * @brief Equality comparison operator between an Optional object and another object.
     *
     * Compares the value contained within an Optional object to another object of
     * potentially different type. The comparison only succeeds if the Optional object
     * contains a value and that value is equal to the other object.
     *
     * @tparam T The type contained by the Optional object.
     * @tparam U The type of the other object to compare with.
     * @param self The Optional object to compare.
     * @param other The other object to compare.
     * @return true if the Optional object contains a value and that value is equal to the other object, otherwise
     * false.
     *
     * @note This operator is marked as constexpr, allowing it to be evaluated at compile time.
     * It is also noexcept, depending on whether the underlying comparison operation is noexcept.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, typename U>
    constexpr bool operator==(const Optional<T> &self, const U &other) noexcept(noexcept(*self == other)) {
        return self.has_value() && *self == other;
    }

    /**
     * @brief Spaceship operator for comparing an Optional object with another value.
     *
     * Provides a three-way comparison between the contents of the Optional and another value
     * of potentially different type. If the Optional has a value, the comparison is made
     * between the contained value and the other using the operator<=>. If the Optional is empty,
     * it returns a result indicating that the Optional is considered greater.
     *
     * @tparam T The type of the value contained in the Optional.
     * @tparam U The type of the value being compared to the Optional's content.
     * @param self The Optional object to be compared.
     * @param other The value to compare against the Optional's content.
     * @return A value of type std::compare_three_way_result_t<T, U> which can be
     * either less, equal, or greater depending on the comparison.
     *
     * @note The operation does not throw exceptions provided that the comparison
     * operation between T and U does not throw.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, typename U>
    constexpr auto operator<=>(const Optional<T> &self, const U &other) noexcept(noexcept(*self <=> other)) {
        using ResultType = std::compare_three_way_result_t<T, U>;
        return self.has_value() ? *self <=> other : ResultType::greater;
    }

    /**
     * @brief Equality operator to compare a value of type U with an Optional of type T.
     *
     * This operator checks if the Optional object contains a value and if that value is
     * equal to the provided object using the equality operator of type U.
     *
     * @param other The object of type U to compare with the Optional's contained value.
     * @param self The Optional object containing a potential value to compare against.
     * @return true if the Optional contains a value and that value is equal to other;
     * false otherwise.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, typename U>
    constexpr bool operator==(const U &other, const Optional<T> &self) noexcept(noexcept(other == *self)) {
        return self.has_value() && other == *self;
    }

    /**
     * @brief Three-way comparison operator for comparing a value of type U with an Optional<T> object.
     *
     * This function performs a three-way comparison between a value of type U and an Optional<T> object.
     * If the Optional<T> has a value, it compares the value with the provided U object using the
     * spaceship operator (<=>). If the Optional<T> does not contain a value, it returns a result that
     * indicates U is considered less than an absent value in the Optional<T>.
     *
     * @tparam T The type contained within the Optional object.
     * @tparam U The type of the value being compared with the value inside the Optional.
     *
     * @param other The value of type U to be compared with the value inside the Optional<T>.
     * @param self The Optional<T> object to compare against.
     *
     * @return A result of type std::compare_three_way_result_t<T, U> indicating the outcome of the comparison.
     *         If the Optional<T> is empty, the result is equivalent to 'less'.
     *
     * @note This operator is constexpr and noexcept, allowing it to be evaluated at compile-time
     *       and ensuring no exceptions are thrown during its execution.
     */
    RETROLIB_EXPORT template <ValidOptionalType T, typename U>
    constexpr auto operator<=>(const U &other, const Optional<T> &self) noexcept(noexcept(other <=> *self)) {
        using ResultType = std::compare_three_way_result_t<T, U>;
        return self.has_value() ? other <=> *self : ResultType::less;
    }

    /**
     * Deduction guide for the Optional class template.
     *
     * This deduction guide allows the compiler to automatically deduce
     * the template argument for the Optional class when an object of type T
     * is provided to the constructor.
     *
     * @tparam T The type of the value that the Optional should store.
     */
    template <ValidOptionalType T>
    Optional(T) -> Optional<T>;

    /**
     * @brief Trait for determining if raw reference optionals are allowed.
     *
     * This specialization inherits from std::true_type, indicating that
     * the Optional type supports raw reference optionals. It is part of
     * the optionals namespace and acts as a compile-time boolean constant.
     */
    template <>
    struct optionals::IsRawReferenceOptionalAllowed<Optional> : std::true_type {};
} // namespace retro