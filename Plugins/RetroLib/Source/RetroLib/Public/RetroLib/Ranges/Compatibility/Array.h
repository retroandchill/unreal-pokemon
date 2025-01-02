// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#ifdef __UNREAL__
#include "RetroLib/Ranges/Compatibility/UnrealContainers.h"

#if !RETROLIB_WITH_MODULES
#include <Containers/Array.h>
#include <Containers/UnrealString.h>
#include <ranges>
#endif

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro::Ranges {

    template <typename C>
    struct TArraySize {
        using Type = typename std::decay_t<C>::SizeType;
    };

    template <>
    struct TArraySize<FString> {
        using Type = decltype(std::declval<FString>().Len());
    };

    template <typename C>
    using TArraySizeType = typename TArraySize<C>::Type;

    template <typename C, typename T, typename S = TArraySizeType<std::decay_t<C>>>
        requires std::is_integral_v<S>
    struct TArrayIterator {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;

        constexpr TArrayIterator() = default;

        constexpr TArrayIterator(T *Ptr, C &Array) : Ptr(Ptr), CurrentArray(&Array), InitialNum(GetNum()) {
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

        constexpr TArrayIterator operator+(S Offset) const {
            return TCheckedArrayIterator(Ptr + Offset, *CurrentArray);
        }

        constexpr friend TArrayIterator operator+(S Offset, const TArrayIterator &Other) {
            return Other + Offset;
        }

        constexpr TArrayIterator operator-(S Offset) const {
            return TCheckedArrayIterator(Ptr - Offset, *CurrentArray);
        }

        constexpr difference_type operator-(const TArrayIterator &Other) const {
            ensureMsgf(CurrentArray->Num() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr - Other.Ptr;
        }

        constexpr bool operator==(const TArrayIterator &Other) const {
            ensureMsgf(CurrentArray->Num() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr == Other.Ptr;
        }

        constexpr std::strong_ordering operator<=>(const TArrayIterator &Other) const {
            ensureMsgf(CurrentArray->Num() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr <=> Other.Ptr;
        }

      private:
        constexpr S GetNum() const {
            if constexpr (UnrealSizedContainer<const C>) {
                return CurrentArray->Num();
            } else if constexpr (UnrealSizedString<const C>) {
                return CurrentArray->Len();
            }
        }

        T *Ptr = nullptr;
        const C *CurrentArray = nullptr;
        S InitialNum = 0;
    };

} // namespace Retro::Ranges

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

RETROLIB_EXPORT template <typename T, typename A>
constexpr T* data(TArray<T, A> &Array) {
    return Array.GetData();
}

RETROLIB_EXPORT template <typename T, typename A>
constexpr const T* data(const TArray<T, A> &Array) {
    return Array.GetData();
}

RETROLIB_EXPORT template <typename T, typename A>
constexpr T* data(TArray<T, A> &&Array) {
    return Array.GetData();
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

RETROLIB_EXPORT inline TCHAR* data(FString &String) {
    return String.GetCharArray().GetData();
}

RETROLIB_EXPORT inline const TCHAR* data(const FString &String) {
    return String.GetCharArray().GetData();
}

RETROLIB_EXPORT inline TCHAR* data(FString &&String) {
    return String.GetCharArray().GetData();
}
#endif