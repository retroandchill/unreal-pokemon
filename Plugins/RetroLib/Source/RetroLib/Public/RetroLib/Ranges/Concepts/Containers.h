/**
 * @file Containers.h
 * @brief Concept definitions regarding containers
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "RetroLib/Ranges/FeatureBridge.h"
#include "RetroLib/TypeTraits.h"

#ifdef __UNREAL__
#include "RetroLib/Ranges/Concepts/Iterable.h"
#endif

#if !RETROLIB_WITH_MODULES
#include <ranges>

#ifdef __UNREAL__
#include "Containers/Map.h"
#include "Traits/IsContiguousContainer.h"
#endif
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

#ifdef __UNREAL__
    /**
     * Concept that defines if a container has an Unreal Engine style reserve method.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept UnrealReservable =
        std::ranges::sized_range<T> && requires(T &Container, std::ranges::range_size_t<T> Size) {
        Container.Reserve(Size);
        Container.Max();
        };

    /**
     * Concept that defines if a container has an Unreal Engine style reserve method for a string.
     *
     * @tparam T The type to check
     */
    template <typename T>
    concept UnrealStringReservable =
        std::ranges::sized_range<T> && requires(T &Container, std::ranges::range_size_t<T> Size) {
        Container.Reserve(Size);
        { Container.GetCharArray() } -> UnrealReservable;
        };
#endif

    /**
     * @class TReservableContainerType
     * @brief Represents a type of container that can be reserved for particular usage.
     *
     * Inherits from InvalidType to provide specialized behavior for reservable
     * containers. This struct is used to define and categorize containers that
     * support reservation functionality, distinguishing them from invalid or
     * non-reservable types.
     */
    RETROLIB_EXPORT template <typename>
    struct TReservableContainerType : FInvalidType {};

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
    struct TReservableContainerType<T> : FValidType {
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

#ifdef __UNREAL__
    /**
     * Provides functionality for reservable container types. Includes methods for reserving storage,
     * querying current capacity, and determining the maximum allowable size for the container.
     *
     * @tparam T The type of container that supports reserving storage and querying capacity.
     */
    RETROLIB_EXPORT template <UnrealReservable T>
    struct TReservableContainerType<T> : FValidType {
        /**
         * Ensures a container has sufficient capacity to accommodate a minimum number of elements.
         *
         * @param Container The container object that requires its capacity to be reserved.
         * @param Size The minimum number of elements for which the container's capacity should be reserved.
         */
        static constexpr void Reserve(T &Container, int32 Size) {
            Container.Reserve(Size);
        }

        /**
         * Retrieves the maximum capacity of the specified container.
         *
         * @param Container The container for which to retrieve the maximum capacity.
         * @return The maximum capacity of the container as an integer.
         */
        static constexpr int32 Capacity(const T &Container) {
            return Container.Max();
        }

        /**
         * Calculates the maximum possible size for a given container type, constrained by its internal limits.
         *
         * @param Container The container whose maximum size is to be determined.
         * @return The maximum size of the container as an integer.
         */
        static constexpr int32 MaxSize(const T &Container) {
            return std::numeric_limits<decltype(Container.Max())>::max();
        }
    };

    /**
     * Provides utility functions for reservable container types to manipulate and query storage capacity.
     *
     * @tparam T The container type that supports reserving storage and querying capacity details.
     */
    RETROLIB_EXPORT template <UnrealStringReservable T>
    struct TReservableContainerType<T> : FValidType {
        /**
         * Allocates sufficient storage capacity for at least the given number of elements
         * in the specified container.
         *
         * @param Container The container object in which storage needs to be reserved.
         * @param Size The minimum number of elements that the container should accommodate.
         */
        static constexpr void Reserve(T &Container, int32 Size) {
            Container.Reserve(Size);
        }

        /**
         * Calculates the storage capacity (in terms of number of elements) for the provided container.
         *
         * @param Container The container whose capacity needs to be determined.
         * @return The maximum number of elements the container can hold based on its current storage.
         */
        static constexpr int32 Capacity(const T &Container) {
            return Container.GetCharArray().Max() / sizeof(typename T::ElementType);
        }

        /**
         * Determines the maximum number of elements that can be held by the provided container.
         *
         * @param Container The container whose maximum capacity is to be computed.
         * @return The maximum number of elements supported by the container type.
         */
        static constexpr int32 MaxSize([[maybe_unused]] const T &Container) {
            return std::numeric_limits<decltype(Container.GetCharArray().Max())>::max();
        }
    };
#endif

    /**
     * Concept that checks if a container has the ability to perform a reserve operation on it.
     *
     * @tparam T The type to check
     */
    RETROLIB_EXPORT template <typename T>
    concept ReservableContainer = std::ranges::sized_range<T> && TReservableContainerType<std::decay_t<T>>::IsValid &&
                                  requires(T &Container, std::ranges::range_size_t<T> Size) {
                                      TReservableContainerType<std::decay_t<T>>::Reserve(Container, Size);
                                      {
                                          TReservableContainerType<std::decay_t<T>>::Capacity(Container)
                                      } -> std::convertible_to<std::ranges::range_size_t<T>>;
                                      {
                                          TReservableContainerType<std::decay_t<T>>::MaxSize(Container)
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
        TReservableContainerType<std::decay_t<T>>::Reserve(Range, Size);
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
        return TReservableContainerType<std::decay_t<T>>::Capacity(Range);
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
        return TReservableContainerType<std::decay_t<T>>::MaxSize(Range);
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

#ifdef __UNREAL__
    /**
     * Concept that defines if a container has an Unreal Engine style Emplace method.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept UnrealEmplace = requires(C &Container, T &&Ref) { Container.Emplace(std::forward<T>(Ref)); };

    /**
     * Concept that defines if a container has an Unreal Engine style Add method.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept UnrealAdd = requires(C &Container, T &&Ref) { Container.Add(std::forward<T>(Ref)); };

    /**
     * Concept that defines if a container has an Unreal Engine style Insert method.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept UnrealInsert = requires(C &Container, T &&Ref) { Container.Insert(std::forward<T>(Ref)); };

    /**
     * Concept that defines if a container has an overloaded += operator and is not part of the STL (i.e. Adding TCHARs to FString).
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept UnrealAddable =
        (!StlAppendable<C, T>) && requires(C &Container, T &&Ref) { Container += std::forward<T>(Ref); };

    /**
     * Concept that defines if a container is part of Unreal Engine and can be emplaced into.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept UnrealAppendable = UnrealEmplace<C, T> || UnrealAdd<C, T> || UnrealInsert<C, T> || UnrealAddable<C, T>;
#endif

    /**
     * Represents a container type that supports appending operations.
     * This structure is intended to be derived from InvalidType and demonstrates
     * a type that can facilitate appending functionality.
     */
    RETROLIB_EXPORT template <typename>
    struct TAppendableContainerType : FInvalidType {};

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
    struct TAppendableContainerType<C> : FValidType {
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

#ifdef __UNREAL__
    /**
     * Provides functionality to append a value to the specified container, using the most appropriate
     * append operation available for the container type and value type.
     *
     * @tparam C The type of the container to which the value will be appended.
     * @param Container The container to which the value will be appended.
     * @param Value The value to be appended to the container.
     * @return The result of the append operation, determined by the specific append method used.
     *
     * @remarks The function determines the appropriate append operation based on the type traits
     * describing the container and value. It supports various operations such as Emplace, Add,
     * Insert, and addition assignment (+=) depending on the container's capabilities.
     */
    template <typename C>
        requires UnrealAppendable<C, std::ranges::range_value_t<C>>
    struct TAppendableContainerType<C> : FValidType {
        /**
         * Appends a value to a container, utilizing the most appropriate insertion method
         * for the container type and the value.
         *
         * Depending on the container's capabilities and the type of the provided value,
         * this function attempts to insert the value using one of the following methods:
         * - Emplace
         * - Add
         * - Insert
         * - Addition operation ("+=")
         *
         * @tparam T The type of the value to be appended to the container.
         * @param Container The target container where the value will be inserted.
         * @param Value The value to be appended to the container.
         * @return The result of the insertion operation, as determined by the specific method used.
         */
        template <typename T>
            requires Retro::Ranges::UnrealAppendable<C, T>
        static constexpr decltype(auto) Append(C &Container, T &&Value) {
            if constexpr (Retro::Ranges::UnrealEmplace<C, T>) {
                return Container.Emplace(std::forward<T>(Value));
            } else if constexpr (Retro::Ranges::UnrealAdd<C, T>) {
                return Container.Add(std::forward<T>(Value));
            } else if constexpr (Retro::Ranges::UnrealInsert<C, T>) {
                return Container.Insert(std::forward<T>(Value));
            } else if constexpr (Retro::Ranges::UnrealAddable<C, T>) {
                return Container += std::forward<T>(Value);
            }
        }
    };

    /**
     * Provides an append operation for `TMap` types, allowing the addition of key-value pairs.
     *
     * The append operation is valid for types that meet all of the following criteria:
     * - Compatible with `Retro::Ranges::UnrealAppendable` for the specified `TMap` type.
     * - Satisfies `TupleLike` requirements for their decayed type.
     * - The tuple size of the decayed type is equal to 2 (representing a key-value pair).
     *
     * @tparam K The key type of the `TMap`.
     * @tparam V The value type of the `TMap`.
     * @tparam A The allocator type of the `TMap`.
     * @tparam F The comparison predicate type of the `TMap`.
     *
     * @param Container A reference to the `TMap` container to which the key-value pair will be appended.
     * @param Value The key-value pair to be appended, represented as a tuple or tuple-like object.
     *              The key is extracted from the first element, and the value from the second element.
     */
    template <typename K, typename V, typename A, typename F>
    struct TAppendableContainerType<TMap<K, V, A, F>> : FValidType {
        /**
         * Appends a key-value pair to the given map container by decomposing the provided tuple-like value.
         *
         * @param Container The map container to which the key-value pair will be appended.
         * @param Value A tuple-like object containing exactly two elements, representing the key and value to be added.
         *        The first element is interpreted as the key, and the second as the value.
         *
         * @return The result of the container's Emplace operation, as determined by the container's implementation.
         */
        template <typename T>
            requires Retro::Ranges::UnrealAppendable<TMap<K, V, A, F>, T> && TupleLike<std::decay_t<T>> &&
                     (std::tuple_size_v<std::decay_t<T>> == 2)
        static constexpr decltype(auto) Append(TMap<K, V, A, F> &Container, T &&Value) {
            Container.Emplace(get<0>(std::forward<T>(Value)), get<1>(std::forward<T>(Value)));
        }
    };
#endif

    /**
     * Concept that defines if a container is appendable.
     *
     * @tparam C The type to check
     * @tparam T the type of element to add
     */
    template <typename C, typename T>
    concept AppendableContainer = TAppendableContainerType<C>::IsValid && requires(C &Container, T &&Value) {
        TAppendableContainerType<C>::Append(Container, std::forward<T>(Value));
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
        return TAppendableContainerType<C>::Append(Container, std::forward<T>(Value));
    }

    /**
     * Concept that defines if a range has a compatible element type.
     *
     * @tparam R The type to check
     * @tparam T the type of element to add
     */
    template <typename R, typename T>
    concept ContainerCompatibleRange = std::ranges::input_range<R> && std::convertible_to<TRangeCommonReference<R>, T>;

#ifdef __UNREAL__
    /**
     * Concept for TArray and other similar sized containers.
     *
     * @tparam C The container
     */
    template <typename C>
    concept UnrealSizedContainer = requires(C &Container) {
        { Container.Num() } -> std::convertible_to<int32>;
    };

    /**
     * Concept for FString since it uses Len instead of Num.
     *
     * @tparam C The container
     */
    template <typename C>
        concept UnrealSizedString = requires(C &Container) {
        { Container.Len() } -> std::convertible_to<int32>;
        };

    /**
     * A helper structure to determine the size type of a given container.
     *
     * This structure extracts and provides a typedef for the size type 'SizeType'
     * of a given container type, where the container type is passed as a template
     * parameter and decayed to ensure proper type handling.
     */
    template <typename C>
    struct TArraySize {
        using Type = typename std::decay_t<C>::SizeType;
    };

    /**
     * Determines the type used to represent the size of an array for the FString type.
     *
     * The Type alias resolves to the type returned by the Len() member function of FString,
     * which represents the number of characters in the string.
     */
    template <>
    struct TArraySize<FString> {
        using Type = decltype(std::declval<FString>().Len());
    };

    /**
     * The size type for the container in question.
     *
     * @tparam C The container
     */
    template <typename C>
    using TArraySizeType = typename TArraySize<C>::Type;

    template <typename C, typename T, typename S = TArraySizeType<std::decay_t<C>>>
        requires std::is_integral_v<S>
    struct TArrayIterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;

        constexpr TArrayIterator() = default;

        constexpr TArrayIterator(T *Ptr, const C &Array) : Ptr(Ptr), CurrentArray(&Array), InitialNum(GetNum()) {
        }

        constexpr T &operator*() const {
            return *Ptr;
        }

        constexpr T *operator->() const {
            return Ptr;
        }

        constexpr TArrayIterator &operator++() {
            ++Ptr;
            return *this;
        }

        constexpr TArrayIterator operator++(int) {
            auto Tmp = *this;
            ++Ptr;
            return Tmp;
        }

        constexpr TArrayIterator &operator--() {
            --Ptr;
            return *this;
        }

        constexpr TArrayIterator operator--(int) {
            auto Tmp = *this;
            --Ptr;
            return Tmp;
        }

        constexpr T &operator[](S Offset) const {
            return Ptr[Offset];
        }

        constexpr TArrayIterator &operator+=(S Offset) {
            Ptr += Offset;
            return *this;
        }

        constexpr TArrayIterator &operator-=(S Offset) {
            Ptr -= Offset;
            return *this;
        }

        constexpr friend TArrayIterator operator+(const TArrayIterator &It, S Offset) {
            return TArrayIterator(It.Ptr + Offset, *It.CurrentArray);
        }

        constexpr friend TArrayIterator operator+(S Offset, const TArrayIterator &Other) {
            return Other + Offset;
        }

        constexpr friend TArrayIterator operator-(const TArrayIterator &It, S Offset) {
            return TArrayIterator(It.Ptr - Offset, *It.CurrentArray);
        }

        constexpr difference_type operator-(const TArrayIterator &Other) const {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr - Other.Ptr;
        }

        constexpr bool operator==(const TArrayIterator &Other) const {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr == Other.Ptr;
        }

        constexpr std::strong_ordering operator<=>(const TArrayIterator &Other) const {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr <=> Other.Ptr;
        }

      private:
        constexpr S GetNum() const {
            if (CurrentArray == nullptr) {
                return 0;
            }

            if constexpr (UnrealSizedContainer<const C>) {
                return CurrentArray->Num();
            } else {
                static_assert(UnrealSizedString<const C>);
                return CurrentArray->Len();
            }
        }

        T *Ptr = nullptr;
        const C *CurrentArray = nullptr;
        S InitialNum = 0;
    };
#endif
} // namespace Retro::Ranges

#ifdef __UNREAL__
RETROLIB_EXPORT template <Retro::Ranges::UnrealSizedContainer R>
constexpr auto size(const R &Range) {
    return Range.Num();
}

RETROLIB_EXPORT template <Retro::Ranges::UnrealSizedString R>
constexpr auto size(const R &Range) {
    return Range.Len();
}

RETROLIB_EXPORT template <Retro::Ranges::CanBridgeToRange I>
constexpr auto begin(I &Range) {
    return Retro::Ranges::TAdapterIterator<Retro::Ranges::TIteratorType<I>, Retro::Ranges::TSentinelType<I>>(Range.begin());
}

RETROLIB_EXPORT template <Retro::Ranges::CanBridgeToRange I>
constexpr auto end(I &Range) {
    return Retro::Ranges::TSentinelAdapter<Retro::Ranges::TIteratorType<I>, Retro::Ranges::TSentinelType<I>>(Range.end());
}

RETROLIB_EXPORT template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<TArray<T, A>>().begin())>)
constexpr auto begin(TArray<T, A> &Array) {
    return Retro::Ranges::TArrayIterator<TArray<T, A>, T>(Array.GetData(), Array);
}

RETROLIB_EXPORT template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<const TArray<T, A>>().begin())>)
constexpr auto begin(const TArray<T, A> &Array) {
    return Retro::Ranges::TArrayIterator<const TArray<T, A>, const T>(Array.GetData(), Array);
}

RETROLIB_EXPORT template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<TArray<T, A>>().end())>)
constexpr auto end(TArray<T, A> &Array) {
    return Retro::Ranges::TArrayIterator<TArray<T, A>, T>(Array.GetData() + Array.Num(), Array);
}

RETROLIB_EXPORT template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<const TArray<T, A>>().end())>)
constexpr auto end(const TArray<T, A> &Array) {
    return Retro::Ranges::TArrayIterator<const TArray<T, A>, const T>(Array.GetData() + Array.Num(), Array);
}

RETROLIB_EXPORT constexpr auto begin(FString &String) {
    if constexpr (std::contiguous_iterator<decltype(String.begin())>) {
        return String.begin();
    } else {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData(), String);
    }
}

RETROLIB_EXPORT constexpr auto begin(const FString &String) {
    if constexpr (std::contiguous_iterator<decltype(String.begin())>) {
        return String.begin();
    } else {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData(), String);
    }
}

RETROLIB_EXPORT constexpr auto end(FString &String) {
    if constexpr (std::contiguous_iterator<decltype(String.end())>) {
        return String.end();
    } else {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData() + String.Len(), String);
    }
}

RETROLIB_EXPORT constexpr auto end(const FString &String) {
    if constexpr (std::contiguous_iterator<decltype(String.end())>) {
        return String.end();
    } else {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData() + String.Len(), String);
    }
}

RETROLIB_EXPORT template <typename T, typename A>
constexpr T *data(TArray<T, A> &Array) {
    return Array.GetData();
}

RETROLIB_EXPORT template <typename T, typename A>
constexpr const T *data(const TArray<T, A> &Array) {
    return Array.GetData();
}

RETROLIB_EXPORT template <typename T, typename A>
constexpr T *data(TArray<T, A> &&Array) {
    return std::move(Array).GetData();
}

RETROLIB_EXPORT inline TCHAR *data(FString &String) {
    return String.GetCharArray().GetData();
}

RETROLIB_EXPORT inline const TCHAR *data(const FString &String) {
    return String.GetCharArray().GetData();
}

RETROLIB_EXPORT inline TCHAR *data(FString &&String) {
    return std::move(String).GetCharArray().GetData();
}
#endif