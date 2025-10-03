// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectIterator.h"

/**
 * Concept that defines an Unreal Engine iterator.
 * @tparam T The type to test
 */
template <typename T>
concept UnrealIterator = requires(const T &Iterator) {
    requires !std::is_void_v<decltype(*Iterator)>;
    *Iterator;
    { !Iterator } -> std::convertible_to<bool>;
} && requires(T &Iterator) { ++Iterator; };

/**
 * Alias for the value obtained from referencing an Unreal iterator.
 * @tparam T The iterator type
 */
template <UnrealIterator T>
using TUnrealIteratorValue = std::decay_t<decltype(*std::declval<T>())>;

/**
 * @class TUnrealIteratorAdapterStorage
 * @brief A utility storage class designed to adapt Unreal Engine iterators for compatibility or extended use.
 *
 * The TUnrealIteratorAdapterStorage class provides a mechanism to encapsulate and manage Unreal Engine iterators.
 * Its primary goal is to serve as an adapter layer, simplifying the integration of Unreal Engine iterator types with
 * other systems or containers that require specialized handling of iterators or iterator data.
 *
 * @tparam T The type the iterator adapter is designed to operate with or inherit behavior from.
 *
 * This class is typically used in scenarios that involve adapting, transforming, or extending the base behavior
 * of Unreal Engine iterator types while ensuring flexibility and type-safety.
 *
 * Key Features:
 * - Facilitates the storage and manipulation of Unreal Engine iterators.
 * - Acts as an adapter layer, enabling compatibility between Unreal Engine iterators and custom implementations.
 * - Provides encapsulation for iterator-related behavior, making it easier to integrate into other systems.
 */
template <UnrealIterator T>
class TUnrealIteratorAdapterStorage
{
  public:
    /**
     * Constructs a TUnrealIteratorAdapterStorage object by forwarding the provided arguments
     * to initialize the internal iterator.
     *
     * @param Args A variadic list of arguments that are forwarded to construct the internal iterator.
     *             The arguments must satisfy the requirements of the iterator's constructor.
     */
    template <typename... A>
        requires std::is_constructible_v<T, A...>
    explicit TUnrealIteratorAdapterStorage(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }

    /**
     * Overload of the not operation to proxy around the iterator's not method.
     * @return The result of the underlying iterators method.
     */
    bool operator!() const
    {
        return !Iterator;
    }

    /**
     * Dereferences the internal iterator to access the element it points to.
     * @return The value that the internal iterator is currently pointing to.
     */
    auto operator*() const
    {
        return *Iterator;
    }

    /**
     * Overload of the operator method to provide a specific operation for this object.
     * @return The result of the defined operation for this operator.
     */
    void operator++()
    {
        ++Iterator;
    }

  private:
    T Iterator;
};

/**
 * Class TUnrealIteratorAdapterStorage provides a storage mechanism for adapting Unreal Engine iterators
 * to custom iterators. This class is designed to facilitate iterator interoperability by providing
 * a storage interface for handling underlying Unreal Engine iterator types seamlessly.
 *
 * This type is usually used internally to manage iterators in a way that abstracts specific Unreal Engine
 * iterator details, enabling compatibility with other iterator systems or patterns.
 */
template <UnrealIterator T>
    requires(!std::is_default_constructible_v<T>)
class TUnrealIteratorAdapterStorage<T>
{
  public:
    /**
     * Default constructor for TUnrealIteratorAdapterStorage.
     * Initializes an instance of the class with no additional setup.
     */
    TUnrealIteratorAdapterStorage() = default;

    /**
     * Constructs a TUnrealIteratorAdapterStorage object by forwarding the provided arguments
     * to initialize the internal iterator.
     *
     * @param Args A variadic list of arguments that are forwarded to construct the internal iterator.
     *             The arguments must satisfy the requirements of the iterator's constructor.
     */
    template <typename... A>
        requires std::is_constructible_v<T, A...>
    explicit TUnrealIteratorAdapterStorage(A &&...Args) : Iterator(InPlace, Forward<A>(Args)...)
    {
    }

    /**
     * Overload of the not operation to proxy around the iterator's not method.
     * @return The result of the underlying iterators method.
     */
    bool operator!() const
    {
        return !*Iterator;
    }

    /**
     * Dereferences the internal iterator to access the element it points to.
     * @return The value that the internal iterator is currently pointing to.
     */
    auto operator*() const
    {
        return **Iterator;
    }

    /**
     * Overload of the operator method to provide a specific operation for this object.
     * @return The result of the defined operation for this operator.
     */
    void operator++()
    {
        ++*Iterator;
    }

  private:
    TOptional<T> Iterator;
};

/**
 * A template adapter class that provides a bridge between Unreal Engine iterators
 * and standard iterator conventions.
 *
 * This adapter allows Unreal Engine style iterators to be used in environments
 * that require standard iterator semantics. It abstracts the underlying Unreal Engine
 * iterator to provide functionalities compatible with standard iterators.
 *
 * Template Parameters:
 * @tparam T The Unreal Engine style iterator type.
 */
template <UnrealIterator T>
class TUnrealIteratorAdapter
{
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = TUnrealIteratorValue<T>;

    /**
     * Constructs a TUnrealIteratorAdapter object with the given arguments,
     * forwarding them to the underlying iterator storage.
     *
     * @param Args The arguments used to initialize the underlying iterator storage.
     */
    template <typename... A>
        requires std::is_constructible_v<TUnrealIteratorAdapterStorage<T>, A...>
    explicit TUnrealIteratorAdapter(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }

    /**
     * Compares the iterator with the standard default sentinel to determine equality.
     * @param Sentinel The standard default sentinel to compare against.
     * @return True if the iterator is not valid (indicating equality with the sentinel), false otherwise.
     */
    bool operator==([[maybe_unused]] const std::default_sentinel_t &Sentinel) const
    {
        return !Iterator;
    }

    /**
     * Dereference operator to access the current element pointed to by the iterator.
     * @return The value obtained by dereferencing the underlying iterator.
     */
    decltype(auto) operator*()
    {
        return *Iterator;
    }
    /**
     * Dereference operator to access the current element pointed to by the iterator.
     * @return The value obtained by dereferencing the underlying iterator.
     */
    decltype(auto) operator*() const
    {
        return *Iterator;
    }

    /**
     * Pre-fix increment operator, used to advance iteration.
     *
     * @return A reference to this iterator.
     */
    TUnrealIteratorAdapter &operator++()
    {
        ++Iterator;
        return *this;
    }

    /**
     * Post-fix increment operator, used to advance iteration.
     */
    void operator++(int)
    {
        ++Iterator;
    }

  private:
    TUnrealIteratorAdapterStorage<T> Iterator;
};

/**
 * A range adapter that provides range support for iterators in the Unreal Engine framework.
 * This class serves as a wrapper to facilitate operations over a given range specified
 * by a pair of iterator objects.
 *
 * The primary purpose of this class is to provide compatibility with ranged-based for loops
 * or other standard library facilities that operate with ranges.
 *
 * @tparam T The type of the iterator used to define the range.
 */
template <UnrealIterator T>
class TUnrealIteratorRange
{
  public:
    /**
     * Constructs a TUnrealIteratorRange using the provided arguments, forwarding them to initialize the underlying
     * iterator.
     * @param Args The variadic arguments used to construct the underlying iterator.
     */
    template <typename... A>
        requires std::constructible_from<T, A...>
    explicit TUnrealIteratorRange(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }

    /**
     * Provides access to the beginning of the iterator range.
     * @return A reference to the underlying iterator.
     */
    TUnrealIteratorAdapter<T> &begin() &
    {
        return Iterator;
    }

    /**
     * Returns the beginning of the iterator range for immutable access.
     * @return A copy of the iterator representing the start of the range.
     */
    TUnrealIteratorAdapter<T> begin() const &
    {
        return Iterator;
    }

    /**
     * Provides an iterator to the beginning of the collection.
     * @return An iterator pointing to the first element of the collection.
     */
    TUnrealIteratorAdapter<T> &&begin() &&
    {
        return MoveTemp(Iterator);
    }

    /**
     * Indicates whether the iterator has reached the end.
     * @return True if the iterator is at the end, otherwise false.
     */
    // ReSharper disable once CppMemberFunctionMayBeStatic
    std::default_sentinel_t end() const
    {
        return std::default_sentinel;
    }

  private:
    TUnrealIteratorAdapter<T> Iterator;
};

/**
 * View for iterating over the fields of a class/struct/etc.
 * @tparam T The type of field
 */
template <typename T>
    requires std::derived_from<T, FField> || std::derived_from<T, UField>
using TFieldView = TUnrealIteratorRange<TFieldIterator<T>>;

/**
 * View for iterating over objects of a specific type.
 * @tparam T The type of object to iterate over.
 */
template <std::derived_from<UObject> T>
using TObjectView = TUnrealIteratorRange<TObjectIterator<T>>;

namespace ranges
{
    template <UnrealIterator T>
    constexpr bool enable_borrowed_range<TUnrealIteratorRange<T>> = true;
}