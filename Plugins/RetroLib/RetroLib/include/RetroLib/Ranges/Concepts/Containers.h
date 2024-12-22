/**
 * @file Containers.h
 * @brief Concept definitions regarding containers
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#if !RETROLIB_WITH_MODULES
#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/TypeTraits.h"

#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace retro::ranges {

    /**
     * Concept that defines if a container has an STL style reserve method.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept StlReservable = std::ranges::sized_range<T> && requires(T &container, std::ranges::range_size_t<T> size) {
        container.reserve(size);
        { container.capacity() } -> std::convertible_to<std::ranges::range_size_t<T>>;
        { container.max_size() } -> std::convertible_to<std::ranges::range_size_t<T>>;
    };

    /**
     * @class ReservableContainerType
     * @brief Represents a type of container that can be reserved for particular usage.
     *
     * Inherits from InvalidType to provide specialized behavior for reservable
     * containers. This struct is used to define and categorize containers that
     * support reservation functionality, distinguishing them from invalid or
     * non-reservable types.
     */
    RETROLIB_EXPORT template <typename>
    struct ReservableContainerType : InvalidType {};

    /**
     * @brief A utility struct that operates on types with reservable storage.
     *
     * ReservableContainerType provides static methods tailored for managing containers
     * that support reservable storage. These containers must conform to the
     * `StlReservable` concept and include methods such as `reserve()` and `capacity()` for
     * manipulating and inspecting their internal memory allocations.
     *
     * @tparam T The type of the container or range this utility operates on.
     */
    RETROLIB_EXPORT template <StlReservable T>
    struct ReservableContainerType<T> : ValidType {
        /**
         * Reserves storage in the given range to accommodate at least the specified number of elements.
         *
         * @param range The container or range object that needs its storage reserved.
         * @param size The minimum number of elements for which storage should be reserved.
         */
        static constexpr void reserve(T &range, std::ranges::range_size_t<T> size) {
            range.reserve(size);
        }

        /**
         * @brief Retrieves the capacity of a given container.
         *
         * This function returns the capacity of the provided container-like object
         * by calling its `capacity` method. It is applicable to any type that
         * defines a `capacity` method.
         *
         * @param range A container-like object that supports the `capacity` method.
         * @return The capacity of the given container.
         */
        static constexpr std::ranges::range_size_t<T> capacity(const T &range) {
            return range.capacity();
        }

        /**
         * @brief Retrieves the maximum size of a given container.
         *
         * This function template returns the maximum number of elements
         * that the specified container type `T` can theoretically contain.
         * It calls the `max_size` member function of the container.
         *
         * @param range The container object for which the maximum size is to be determined.
         *
         * @return The maximum size of the container as returned by its `max_size` method.
         */
        static constexpr std::ranges::range_size_t<T> max_size(const T &range) {
            return range.max_size();
        }
    };

    /**
     * Concept that checks if a container has the ability to perform a reserve operation on it.
     *
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept ReservableContainer = std::ranges::sized_range<T> && ReservableContainerType<std::decay_t<T>>::is_valid &&
                                  requires(T &container, std::ranges::range_size_t<T> size) {
                                      ReservableContainerType<std::decay_t<T>>::reserve(container, size);
                                      {
                                          ReservableContainerType<std::decay_t<T>>::capacity(container)
                                      } -> std::convertible_to<std::ranges::range_size_t<T>>;
                                      {
                                          ReservableContainerType<std::decay_t<T>>::max_size(container)
                                      } -> std::convertible_to<std::ranges::range_size_t<T>>;
                                  };

    /**
     * Reserves storage in the given range to accommodate at least the specified number of elements.
     *
     * @tparam T The type of container
     * @param range The container or range object that needs its storage reserved.
     * @param size The minimum number of elements for which storage should be reserved.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr void container_reserve(T &range, std::ranges::range_size_t<T> size) {
        ReservableContainerType<std::decay_t<T>>::reserve(range, size);
    }

    /**
     * @brief Retrieves the capacity of a given container.
     *
     * This function returns the capacity of the provided container-like object
     * by calling its `capacity` method. It is applicable to any type that
     * defines a `capacity` method.
     *
     * @tparam T The type of container
     * @param range A container-like object that supports the `capacity` method.
     * @return The capacity of the given container.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr std::ranges::range_size_t<T> container_capacity(const T &range) {
        return ReservableContainerType<std::decay_t<T>>::capacity(range);
    }

    /**
     * @brief Retrieves the maximum size of a given container.
     *
     * This function template returns the maximum number of elements
     * that the specified container type `T` can theoretically contain.
     * It calls the `max_size` member function of the container.
     *
     * @tparam T The type of the container which should provide a `max_size` method.
     * @param range The container object for which the maximum size is to be determined.
     *
     * @return The maximum size of the container as returned by its `max_size` method.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr std::ranges::range_size_t<T> container_max_size(const T &range) {
        return ReservableContainerType<std::decay_t<T>>::max_size(range);
    }

    /**
     * Concept that defines if a container has an STL style emplace_back method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlEmplaceBack = requires(C &c, R &&ref) { c.emplace_back(std::forward<R>(ref)); };

    /**
     * Concept that defines if a container has an STL style push_back method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlPushBack = requires(C &c, R &&ref) { c.push_back(std::forward<R>(ref)); };

    /**
     * Concept that defines if a container has an STL style emplace method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlEmplace = requires(C &c, R &&ref) { c.emplace(std::forward<R>(ref)); };

    /**
     * Concept that defines if a container has an STL style insert method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlInsert = requires(C &c, R &&ref) { c.insert(std::forward<R>(ref)); };

    /**
     * Concept that defines if a container has any compliant STL-style append methods
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlAppendable = StlEmplaceBack<C, R> || StlPushBack<C, R> || StlEmplace<C, R> || StlInsert<C, R>;

    /**
     * Represents a container type that supports appending operations.
     * This structure is intended to be derived from InvalidType and demonstrates
     * a type that can facilitate appending functionality.
     */
    RETROLIB_EXPORT template <typename>
    struct AppendableContainerType : InvalidType {};

    /**
     * Provides a type trait capable of adding elements to a container by leveraging
     * various appending mechanisms available in the standard library or container API.
     *
     * The `AppendableContainerType` struct is designed to determine and utilize the
     * most efficient and compatible method for appending elements to a container,
     * such as `emplace_back`, `push_back`, `emplace`, or `insert`, based on the
     * properties of the container and the operations it supports.
     *
     * @tparam C The container type to which the append functionality is being applied.
     * This container must satisfy the requirements enforced by the `ValidType` base
     * and must support one or more appending methods (e.g., `emplace_back`, `push_back`,
     * `emplace`, or `insert`).
     */
    RETROLIB_EXPORT template <typename C>
        requires std::ranges::range<C> && StlAppendable<C, std::ranges::range_value_t<C>>
    struct AppendableContainerType<C> : ValidType {
        /**
         * Appends a value to the given container using the most appropriate method
         * available, such as emplace_back, push_back, emplace, or insert.
         *
         * This function utilizes compile-time checks to determine the most suitable
         * method to add an element to the container, thus ensuring both efficiency
         * and compatibility with the container's capabilities.
         *
         * @param container The container to which the value is to be appended. It
         * should support one of the following operations: emplace_back, push_back,
         * emplace, or insert.
         * @param value The value to be appended to the container. The type of this
         * value must be compatible with the container's value type and the supported
         * insertion methods.
         * @return This function returns the result of the container's append operation.
         * The exact return type depends on the append method used (e.g., emplace_back,
         * push_back, emplace, or insert). Note that the return type could vary, such
         * as void or an iterator, depending on the method invoked.
         */
        template <typename T>
            requires StlAppendable<C, T>
        static constexpr decltype(auto) append(C &container, T &&value) {
            if constexpr (StlEmplaceBack<C, T>) {
                return container.emplace_back(std::forward<T>(value));
            } else if constexpr (StlPushBack<C, T>) {
                return container.push_back(std::forward<T>(value));
            } else if constexpr (StlEmplace<C, T>) {
                return container.emplace(std::forward<T>(value));
            } else if constexpr (StlInsert<C, T>) {
                return container.insert(std::forward<T>(value));
            }
        }
    };

    /**
     * Concept that defines if a container is appendable.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept AppendableContainer = AppendableContainerType<C>::is_valid && requires(C &container, T &&value) {
        AppendableContainerType<C>::append(container, std::forward<T>(value));
    };

    /**
     * Appends a value to the given container using the most appropriate method
     * available, such as emplace_back, push_back, emplace, or insert.
     *
     * This function utilizes compile-time checks to determine the most suitable
     * method to add an element to the container, thus ensuring both efficiency
     * and compatibility with the container's capabilities.
     *
     * @param container The container to which the value is to be appended. It
     * should support one of the following operations: emplace_back, push_back,
     * emplace, or insert.
     * @param value The value to be appended to the container. The type of this
     * value must be compatible with the container's value type and the supported
     * insertion methods.
     * @return This function returns the result of the container's append operation.
     * The exact return type depends on the append method used (e.g., emplace_back,
     * push_back, emplace, or insert). Note that the return type could vary, such
     * as void or an iterator, depending on the method invoked.
     */
    template <typename C, typename T>
        requires AppendableContainer<C, T>
    constexpr decltype(auto) append_container(C &container, T &&value) {
        return AppendableContainerType<C>::append(container, std::forward<T>(value));
    }

    /**
     * Concept that defines if a range has a compatible element type.
     *
     * @tparam R The type to check
     * @tparam T the type of element to add
     */
    template <typename R, typename T>
    concept ContainerCompatibleRange = std::ranges::input_range<R> && std::convertible_to<RangeCommonReference<R>, T>;
} // namespace retro::ranges