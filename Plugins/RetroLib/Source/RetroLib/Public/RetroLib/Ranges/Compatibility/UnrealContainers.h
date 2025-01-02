// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#if !RETROLIB_WITH_MODULES
#include "Containers/Map.h"
#include "Traits/IsContiguousContainer.h"
#endif

#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Compatibility/ForEachRange.h"
#include "RetroLib/TypeTraits.h"
#include "RetroLib/Ranges/Concepts/Containers.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    template <typename C, typename T>
    concept UnrealEmplace = requires(C &Container, T &&Ref) { Container.Emplace(std::forward<T>(Ref)); };

    template <typename C, typename T>
    concept UnrealAdd = requires(C &Container, T &&Ref) { Container.Add(std::forward<T>(Ref)); };

    template <typename C, typename T>
    concept UnrealInsert = requires(C &Container, T &&Ref) { Container.Insert(std::forward<T>(Ref)); };

    template <typename C, typename T>
    concept UnrealAddable = (!StlAppendable<C, T>) && requires(C &Container, T &&Ref) { Container += std::forward<T>(Ref); };

    template <typename C, typename T>
    concept UnrealAppendable = UnrealEmplace<C, T> || UnrealAdd<C, T> || UnrealInsert<C, T> || UnrealAddable<C, T>;

    template <typename C>
    concept UnrealSizedContainer = requires(C &Container) {
        { Container.Num() } -> std::convertible_to<int32>;
    };

    template <typename C>
    concept UnrealSizedString = requires(C &Container) {
        { Container.Len() } -> std::convertible_to<int32>;
    };

    template <typename T>
    concept UnrealReservable =
        std::ranges::sized_range<T> && requires(T &Container, std::ranges::range_size_t<T> Size) {
            Container.Reserve(Size);
            Container.Max();
        };

    template <typename T>
    concept UnrealStringReservable =
        std::ranges::sized_range<T> && requires(T &Container, std::ranges::range_size_t<T> Size) {
            Container.Reserve(Size);
            Container.GetAllocatedSize();
        };

    template <>
    struct TIsMap<TMap> : std::true_type {};
} // namespace Retro::Ranges

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
    return Retro::Ranges::TAdapterIterator<Retro::TIteratorType<I>, Retro::TSentinelType<I>>(Range.begin());
}

RETROLIB_EXPORT template <Retro::Ranges::CanBridgeToRange I>
constexpr auto end(I &Range) {
    return Retro::Ranges::TSentinelAdapter<Retro::TIteratorType<I>, Retro::TSentinelType<I>>(Range.end());
}

namespace Retro::Ranges {
    template <typename C>
        requires UnrealAppendable<C, std::ranges::range_value_t<C>>
    struct TAppendableContainerType<C> : FValidType {
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

    template <typename K, typename V, typename A, typename F>
    struct TAppendableContainerType<TMap<K, V, A, F>> : FValidType {
        template <typename T>
            requires Retro::Ranges::UnrealAppendable<TMap<K, V, A, F>, T> && TupleLike<std::decay_t<T>> &&
                     (std::tuple_size_v<std::decay_t<T>> == 2)
        static constexpr decltype(auto) Append(TMap<K, V, A, F> &Container, T &&Value) {
            Container.Emplace(get<0>(std::forward<T>(Value)), get<1>(std::forward<T>(Value)));
        }
    };

    RETROLIB_EXPORT template <UnrealReservable T>
    struct TReservableContainerType<T> : FValidType {
        static constexpr void Reserve(T &Container, int32 Size) {
            Container.Reserve(Size);
        }

        static constexpr int32 Capacity(const T &Container) {
            return Container.Max();
        }

        static constexpr int32 MaxSize(const T &Container) {
            return std::numeric_limits<decltype(Container.Max())>::max();
        }
    };

    RETROLIB_EXPORT template <UnrealStringReservable T>
        requires(!UnrealReservable<T>)
    struct TReservableContainerType<T> : FValidType {
        static constexpr void Reserve(T &Container, int32 Size) {
            Container.Reserve(Size);
        }

        static constexpr int32 Capacity(const T &Container) {
            return Container.GetAllocatedSize() / sizeof(typename T::ElementType);
        }

        static constexpr int32 MaxSize([[maybe_unused]] const T &Container) {
            return std::numeric_limits<decltype(Container.GetAllocatedSize())>::max();
        }
    };
} // namespace Retro::Ranges
#endif