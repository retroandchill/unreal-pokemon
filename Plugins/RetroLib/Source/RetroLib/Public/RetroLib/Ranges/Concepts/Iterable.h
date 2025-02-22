/**
 * @file Iterable.h
 * @brief Concepts pertaining to iterability.
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */

#pragma once

#if !RETROLIB_WITH_MODULES
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {
    /**
     * Concept for the minimum requirements for an iterator.
     *
     * @tparam T The iterator type
     */
    template <typename T>
    concept Iterator = requires(T Iter) {
        { ++Iter } -> std::same_as<T &>;
        *Iter;
    };

    /**
     * Concept for the minimum requirements for an iterator sentinel.
     *
     * @tparam S The sentinel type
     * @tparam I The iterator type
     */
    template <typename S, typename I>
    concept Sentinel = requires(S Sentinel, I Iterator) {
        { Iterator != Sentinel } -> std::same_as<bool>;
    };

    /**
     * The iterator type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using TIteratorType = std::decay_t<decltype(std::declval<T>().begin())>;

    /**
     * The sentinel type from a container.
     *
     * @tparam T The container type.
     */
    template <typename T>
    using TSentinelType = std::decay_t<decltype(std::declval<T>().end())>;

    /**
     * The concept for checking if container has iterator support.
     *
     * @tparam T The container type.
     */
    template <typename T>
    concept Iterable = requires(T &&Range) {
        { std::forward<T>(Range).begin() } -> Iterator;
        { std::forward<T>(Range).end() } -> Sentinel<TIteratorType<T>>;
    };

    /**
     * Concept for checking if an iterator can be bridged into being used for C++ 20 ranges.
     */
    RETROLIB_EXPORT template <typename I>
    concept BridgableIterator = !std::input_or_output_iterator<I> && std::is_trivially_destructible_v<I> &&
                                std::is_trivially_copy_constructible_v<I> && std::is_trivially_move_constructible_v<I>;

    /**
     * @brief Represents a storage for iterator-like objects.
     *
     * This structure encapsulates an iterator or adapter-type object
     * and provides constructors for initializing the storage.
     */
    template <typename I>
    struct TIteratorStorage {
        I Adapted;

        /**
         * @brief Default constructor for the IteratorStorage class.
         *
         * This constructor initializes an IteratorStorage object to its default state.
         */
        constexpr TIteratorStorage() = default;

        /**
         * @brief Constructs an IteratorStorage object with a provided adapted object.
         *
         * This constructor initializes the IteratorStorage object by forwarding the provided argument
         * to the internal adapted member.
         *
         * @tparam T The type of the adapted object.
         * @param adapted The object to be stored within the IteratorStorage.
         */
        template <typename T>
            requires std::constructible_from<I, T> && (!std::same_as<std::decay_t<T>, TIteratorStorage>)
        constexpr explicit TIteratorStorage(T &&adapted) : Adapted(std::forward<T>(adapted)) {
        }
    };

    /**
     * @brief A template structure for storing an iterator or an empty state.
     *
     * Provides the ability to store either an iterator of type `I` or an empty state
     * using a union. This is useful for adapting iterator-like objects.
     *
     * @tparam I The type of the iterator or adapted object to be stored.
     */
    template <typename I>
        requires(!std::is_default_constructible_v<I>)
    struct TIteratorStorage<I> {
        union {
            std::monostate Empty;
            I Adapted;
        };

        /**
         * @brief Constructs an IteratorStorage object with an empty state.
         *
         * Initializes an IteratorStorage instance, ensuring that it starts in an empty state.
         *
         * @return A constexpr IteratorStorage instance in an empty state.
         */
        constexpr TIteratorStorage() : Empty(std::monostate{}) {
        }

        /**
         * @brief Explicit constructor for the IteratorStorage class.
         *
         * Initializes an IteratorStorage object with the provided adapted object, using perfect forwarding.
         *
         * @param Adapted The object to adapt, passed as an rvalue reference.
         */
        template <typename T>
            requires std::constructible_from<I, T> && (!std::same_as<std::decay_t<T>, TIteratorStorage>)
        constexpr explicit TIteratorStorage(T &&Adapted) : Adapted(std::forward<T>(Adapted)) {
        }
    };

    /**
     * @brief Adapter class for managing iterator assignments.
     *
     * This class provides additional functionalities for adapting iterators
     * with custom assignment operations. It extends the `IteratorStorage<I>`
     * class to include explicit copy and move assignment operators.
     */
    template <BridgableIterator I>
    struct TIteratorAssignAdapter : TIteratorStorage<I> {
        /**
         * @brief Default constructor for the IteratorAssignAdapter class.
         *
         * This constructor initializes an IteratorAssignAdapter object with its default state.
         */
        constexpr TIteratorAssignAdapter() = default;

        /**
         * @brief Copy constructor for the IteratorAssignAdapter class.
         *
         * This constructor creates a new IteratorAssignAdapter object as a copy of an existing one.
         * The noexcept specification is determined by the nothrow copy constructibility of the template parameter I.
         *
         * @param other The IteratorAssignAdapter instance to copy.
         */
        constexpr TIteratorAssignAdapter(const TIteratorAssignAdapter &) noexcept(
            std::is_nothrow_copy_constructible_v<I>) = default;

        /**
         * @brief Move constructor for the IteratorAssignAdapter class.
         *
         * This constructor performs a move operation, enabling the transfer of resources
         * from another IteratorAssignAdapter instance.
         *
         * @param other The IteratorAssignAdapter instance to be moved.
         * @return A new IteratorAssignAdapter object with resources moved from the input instance.
         */
        constexpr TIteratorAssignAdapter(TIteratorAssignAdapter &&) noexcept(std::is_nothrow_move_constructible_v<I>) =
            default;

        /**
         * @brief Default destructor for the IteratorAssignAdapter class.
         *
         * This destructor allows proper cleanup of an IteratorAssignAdapter object and its associated resources.
         */
        ~TIteratorAssignAdapter() = default;

        using TIteratorStorage<I>::TIteratorStorage;

        using TIteratorStorage<I>::Adapted;

        /**
         * @brief Copy assignment operator for the IteratorAssignAdapter class.
         *
         * This operator assigns the state of another IteratorAssignAdapter object to the current instance.
         *
         * @param Other The IteratorAssignAdapter instance whose state is to be assigned.
         * @return A reference to the current instance with the updated state.
         */
        constexpr TIteratorAssignAdapter &
        operator=(const TIteratorAssignAdapter &Other) noexcept(std::is_nothrow_copy_constructible_v<I>) {
            new (&Adapted) I(Other.Adapted);
            return *this;
        }

        /**
         * @brief Move assignment operator for the IteratorAssignAdapter class.
         *
         * This operator moves the state of another IteratorAssignAdapter object into the current object.
         * The move is performed without throwing exceptions.
         *
         * @param Other The IteratorAssignAdapter object to move from.
         * @return A reference to the current IteratorAssignAdapter object.
         */
        constexpr TIteratorAssignAdapter &
        operator=(TIteratorAssignAdapter &&Other) noexcept(std::is_nothrow_move_constructible_v<I>) {
            new (&Adapted) I(std::move(Other.Adapted));
            return *this;
        }
    };

    template <typename I>
    using TIteratorAssignLayer = std::conditional_t<std::movable<I>, TIteratorStorage<I>, TIteratorAssignAdapter<I>>;

    RETROLIB_EXPORT template <Iterator I, Sentinel<I> S>
    class TAdapterIterator;

    /**
     * @brief The SentinelAdapter class is a private adapter wrapper for handling sentinel functionality.
     *
     * This class inherits from IteratorAssignLayer and provides functionality to manage iterators
     * with a specific focus on adapting sentinels.
     */
    RETROLIB_EXPORT template <Iterator I, Sentinel<I> S>
    class TSentinelAdapter : private TIteratorAssignLayer<S> {
        using Base = TIteratorAssignLayer<S>;

      public:
        /**
         * @brief Default constructor for the SentinelAdapter class.
         *
         * This constructor initializes a SentinelAdapter object with its default settings.
         * It leverages the default constructor from its base class.
         */
        constexpr TSentinelAdapter() = default;

        using Base::Base;

      private:
        using Base::Adapted;

        friend class TAdapterIterator<I, S>;
    };

    /**
     * @class TAdapterIterator
     * @brief Provides an adapter iterator that transforms input iterators into a new form
     * while maintaining iterator semantics.
     *
     * AdapterIterator serves as a flexible utility to adapt and manipulate iterators for
     * varied use-cases. It is implemented as a derived class from IteratorAssignLayer,
     * and provides functionality consistent with a standard input iterator.
     *
     * @tparam I The type of the base iterator being adapted.
     *
     * Features:
     * - Supports input iterator category.
     * - Provides custom equality comparison with `SentinelAdapter`.
     * - Allows dereferencing of the adapted iterator to obtain the current element.
     * - Supports both pre-increment and post-increment operations.
     *
     * Important:
     * - The iterator adapts the behavior of the base iterator `I`, ensuring seamless
     *   functionality in different contexts.
     * - The equality operator `operator==` is specifically designed for comparison with
     *   `SentinelAdapter<I, S>` rather than another `AdapterIterator`.
     */
    template <Iterator I, Sentinel<I> S>
    class TAdapterIterator : private TIteratorAssignLayer<I> {
        using Base = TIteratorAssignLayer<I>;

      public:
        /**
         * Specifies the iterator as an input iterator.
         */
        using iterator_category = std::input_iterator_tag;

        /**
         * The type of value yielded by dereferencing the iterator.
         */
        using value_type = std::remove_reference_t<TDereferencedType<I>>;

        /**
         * The type used to represent distances between iterators.
         */
        using difference_type = std::ptrdiff_t;
        /**
         * Pointer type to the `value_type`.
         */
        using pointer = value_type *;

        /**
         * Reference type to the `value_type`.
         */
        using reference = value_type &;

        /**
         * @brief Default constructor for the AdapterIterator class.
         *
         * This constructor initializes an AdapterIterator object with its default settings.
         *
         * @return A default-constructed AdapterIterator object.
         */
        constexpr TAdapterIterator() = default;

        using Base::Base;

        /**
         * @brief Compares the current SentinelAdapter with another SentinelAdapter for equality.
         *
         * This operator checks if the adapted member of the current SentinelAdapter
         * is equal to the adapted member of the provided SentinelAdapter.
         *
         * @param Sentinel The SentinelAdapter to compare with the current object.
         * @return True if the adapted members are equal, otherwise false.
         */
        constexpr bool operator==(const TSentinelAdapter<I, S> &Sentinel) const {
            return !(Adapted != Sentinel.Adapted);
        }

        /**
         * @brief Dereference operator for accessing the value pointed to by the adapted iterator.
         *
         * This operator returns a reference or value of the element currently pointed to by the adapted iterator.
         *
         * @return The dereferenced value or reference obtained from the adapted iterator.
         */
        constexpr decltype(auto) operator*() const {
            return *Adapted;
        }

        /**
         * @brief Pre-increment operator for the AdapterIterator class.
         *
         * Advances the internal adapted iterator and returns a reference to the updated AdapterIterator object.
         *
         * @return A reference to the incremented AdapterIterator object.
         */
        constexpr TAdapterIterator &operator++() {
            ++Adapted;
            return *this;
        }

        /**
         * @brief Post-increment operator for the adapted iterator.
         *
         * This operator advances the adapted iterator to the next position.
         *
         * @param int Unused parameter to distinguish post-increment from pre-increment.
         */
        void operator++(int) {
            ++Adapted;
        }

      private:
        using Base::Adapted;
    };

    /**
     * Concept to check if an iterable container can be bridged into a range.
     */
    RETROLIB_EXPORT template <typename I>
    concept CanBridgeToRange = Iterable<I> && BridgableIterator<TIteratorType<I>>;
} // namespace Retro::Ranges