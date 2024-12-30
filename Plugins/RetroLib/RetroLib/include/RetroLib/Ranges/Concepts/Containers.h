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

namespace Retro::Ranges {

    /**
     * Concept that defines if a container has an STL style reserve method.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept StlReservable = std::ranges::sized_range<T> && requires(T &Container, std::ranges::range_size_t<T> Size) {
        Container.reserve(Size);
        { Container.capacity() } -> std::convertible_to<std::ranges::range_size_t<T>>;
        { Container.max_size() } -> std::convertible_to<std::ranges::range_size_t<T>>;
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
         * @param Range The container or range object that needs its storage reserved.
         * @param Size The minimum number of elements for which storage should be reserved.
         */
        static constexpr void Reserve(T &Range, std::ranges::range_size_t<T> Size) {
            Range.reserve(Size);
        }

        /**
         * @brief Retrieves the capacity of a given container.
         *
         * This function returns the capacity of the provided container-like object
         * by calling its `capacity` method. It is applicable to any type that
         * defines a `capacity` method.
         *
         * @param Range A container-like object that supports the `capacity` method.
         * @return The capacity of the given container.
         */
        static constexpr std::ranges::range_size_t<T> Capacity(const T &Range) {
            return Range.capacity();
        }

        /**
         * @brief Retrieves the maximum size of a given container.
         *
         * This function template returns the maximum number of elements
         * that the specified container type `T` can theoretically contain.
         * It calls the `max_size` member function of the container.
         *
         * @param Range The container object for which the maximum size is to be determined.
         *
         * @return The maximum size of the container as returned by its `max_size` method.
         */
        static constexpr std::ranges::range_size_t<T> MaxSize(const T &Range) {
            return Range.max_size();
        }
    };

    /**
     * Concept that checks if a container has the ability to perform a reserve operation on it.
     *
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept ReservableContainer = std::ranges::sized_range<T> && ReservableContainerType<std::decay_t<T>>::IsValid &&
                                  requires(T &Container, std::ranges::range_size_t<T> Size) {
                                      ReservableContainerType<std::decay_t<T>>::Reserve(Container, Size);
                                      {
                                          ReservableContainerType<std::decay_t<T>>::Capacity(Container)
                                      } -> std::convertible_to<std::ranges::range_size_t<T>>;
                                      {
                                          ReservableContainerType<std::decay_t<T>>::MaxSize(Container)
                                      } -> std::convertible_to<std::ranges::range_size_t<T>>;
                                  };

    /**
     * Reserves storage in the given range to accommodate at least the specified number of elements.
     *
     * @tparam T The type of container
     * @param Range The container or range object that needs its storage reserved.
     * @param Size The minimum number of elements for which storage should be reserved.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr void ContainerReserve(T &Range, std::ranges::range_size_t<T> Size) {
        ReservableContainerType<std::decay_t<T>>::Reserve(Range, Size);
    }

    /**
     * @brief Retrieves the capacity of a given container.
     *
     * This function returns the capacity of the provided container-like object
     * by calling its `capacity` method. It is applicable to any type that
     * defines a `capacity` method.
     *
     * @tparam T The type of container
     * @param Range A container-like object that supports the `capacity` method.
     * @return The capacity of the given container.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr std::ranges::range_size_t<T> ContainerCapacity(const T &Range) {
        return ReservableContainerType<std::decay_t<T>>::Capacity(Range);
    }

    /**
     * @brief Retrieves the maximum size of a given container.
     *
     * This function template returns the maximum number of elements
     * that the specified container type `T` can theoretically contain.
     * It calls the `max_size` member function of the container.
     *
     * @tparam T The type of the container which should provide a `max_size` method.
     * @param Range The container object for which the maximum size is to be determined.
     *
     * @return The maximum size of the container as returned by its `max_size` method.
     */
    RETROLIB_EXPORT template <ReservableContainer T>
    constexpr std::ranges::range_size_t<T> ContainerMaxSize(const T &Range) {
        return ReservableContainerType<std::decay_t<T>>::MaxSize(Range);
    }

    /**
     * Concept that defines if a container has an STL style emplace_back method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlEmplaceBack = requires(C &Container, R &&Ref) { Container.emplace_back(std::forward<R>(Ref)); };

    /**
     * Concept that defines if a container has an STL style push_back method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlPushBack = requires(C &Container, R &&Ref) { Container.push_back(std::forward<R>(Ref)); };

    /**
     * Concept that defines if a container has an STL style emplace method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlEmplace = requires(C &Container, R &&Ref) { Container.emplace(std::forward<R>(Ref)); };

    /**
     * Concept that defines if a container has an STL style insert method.
     *
     * @tparam C The type to check
     * @tparam R the type of element to add
     */
    template <typename C, typename R>
    concept StlInsert = requires(C &Container, R &&Ref) { Container.insert(std::forward<R>(Ref)); };

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
         * @param Container The container to which the value is to be appended. It
         * should support one of the following operations: emplace_back, push_back,
         * emplace, or insert.
         * @param Value The value to be appended to the container. The type of this
         * value must be compatible with the container's value type and the supported
         * insertion methods.
         * @return This function returns the result of the container's append operation.
         * The exact return type depends on the append method used (e.g., emplace_back,
         * push_back, emplace, or insert). Note that the return type could vary, such
         * as void or an iterator, depending on the method invoked.
         */
        template <typename T>
            requires StlAppendable<C, T>
        static constexpr decltype(auto) Append(C &Container, T &&Value) {
            if constexpr (StlEmplaceBack<C, T>) {
                return Container.emplace_back(std::forward<T>(Value));
            } else if constexpr (StlPushBack<C, T>) {
                return Container.push_back(std::forward<T>(Value));
            } else if constexpr (StlEmplace<C, T>) {
                return Container.emplace(std::forward<T>(Value));
            } else if constexpr (StlInsert<C, T>) {
                return Container.insert(std::forward<T>(Value));
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
    concept AppendableContainer = AppendableContainerType<C>::IsValid && requires(C &Container, T &&Value) {
        AppendableContainerType<C>::Append(Container, std::forward<T>(Value));
    };

    /**
     * Appends a value to the given container using the most appropriate method
     * available, such as emplace_back, push_back, emplace, or insert.
     *
     * This function utilizes compile-time checks to determine the most suitable
     * method to add an element to the container, thus ensuring both efficiency
     * and compatibility with the container's capabilities.
     *
     * @param Container The container to which the value is to be appended. It
     * should support one of the following operations: emplace_back, push_back,
     * emplace, or insert.
     * @param Value The value to be appended to the container. The type of this
     * value must be compatible with the container's value type and the supported
     * insertion methods.
     * @return This function returns the result of the container's append operation.
     * The exact return type depends on the append method used (e.g., emplace_back,
     * push_back, emplace, or insert). Note that the return type could vary, such
     * as void or an iterator, depending on the method invoked.
     */
    template <typename C, typename T>
        requires AppendableContainer<C, T>
    constexpr decltype(auto) AppendContainer(C &Container, T &&Value) {
        return AppendableContainerType<C>::Append(Container, std::forward<T>(Value));
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