/**
 * @file UnrealContainers.h
 * @brief Concept definitions regarding containers
 *
 * @author Retro & Chill
 * https://github.com/retroandchill
 */
#pragma once

#include "Containers/Map.h"
#include "Iterable.h"
#include "Traits/IsContiguousContainer.h"

namespace Retro::Ranges
{
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
    struct TArraySize
    {
        using Type = std::remove_cvref_t<C>::SizeType;
    };

    /**
     * Determines the type used to represent the size of an array for the FString type.
     *
     * The Type alias resolves to the type returned by the Len() member function of FString,
     * which represents the number of characters in the string.
     */
    template <>
    struct TArraySize<FString>
    {
        using Type = decltype(std::declval<FString>().Len());
    };

    /**
     * The size type for the container in question.
     *
     * @tparam C The container
     */
    template <typename C>
    using TArraySizeType = TArraySize<C>::Type;

    template <typename C, typename T, typename S = TArraySizeType<std::remove_cvref_t<C>>>
        requires std::is_integral_v<S>
    struct TArrayIterator
    {
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::contiguous_iterator_tag;

        constexpr TArrayIterator() = default;

        constexpr TArrayIterator(T *Ptr, const C &Array) : Ptr(Ptr), CurrentArray(&Array), InitialNum(GetNum())
        {
        }

        constexpr T &operator*() const
        {
            return *Ptr;
        }

        constexpr T *operator->() const
        {
            return Ptr;
        }

        constexpr TArrayIterator &operator++()
        {
            ++Ptr;
            return *this;
        }

        constexpr TArrayIterator operator++(int)
        {
            auto Tmp = *this;
            ++Ptr;
            return Tmp;
        }

        constexpr TArrayIterator &operator--()
        {
            --Ptr;
            return *this;
        }

        constexpr TArrayIterator operator--(int)
        {
            auto Tmp = *this;
            --Ptr;
            return Tmp;
        }

        constexpr T &operator[](S Offset) const
        {
            return Ptr[Offset];
        }

        constexpr TArrayIterator &operator+=(S Offset)
        {
            Ptr += Offset;
            return *this;
        }

        constexpr TArrayIterator &operator-=(S Offset)
        {
            Ptr -= Offset;
            return *this;
        }

        constexpr friend TArrayIterator operator+(const TArrayIterator &It, S Offset)
        {
            return TArrayIterator(It.Ptr + Offset, *It.CurrentArray);
        }

        constexpr friend TArrayIterator operator+(S Offset, const TArrayIterator &Other)
        {
            return Other + Offset;
        }

        constexpr friend TArrayIterator operator-(const TArrayIterator &It, S Offset)
        {
            return TArrayIterator(It.Ptr - Offset, *It.CurrentArray);
        }

        constexpr difference_type operator-(const TArrayIterator &Other) const
        {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr - Other.Ptr;
        }

        constexpr bool operator==(const TArrayIterator &Other) const
        {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr == Other.Ptr;
        }

        constexpr std::strong_ordering operator<=>(const TArrayIterator &Other) const
        {
            ensureMsgf(GetNum() == InitialNum, TEXT("Array has changed during ranged-for iteration!"));
            return Ptr <=> Other.Ptr;
        }

      private:
        constexpr S GetNum() const
        {
            if (CurrentArray == nullptr)
            {
                return 0;
            }

            if constexpr (UnrealSizedContainer<const C>)
            {
                return CurrentArray->Num();
            }
            else
            {
                static_assert(UnrealSizedString<const C>);
                return CurrentArray->Len();
            }
        }

        T *Ptr = nullptr;
        const C *CurrentArray = nullptr;
        S InitialNum = 0;
    };
} // namespace Retro::Ranges

template <Retro::Ranges::UnrealSizedContainer R>
constexpr auto size(const R &Range)
{
    return Range.Num();
}

template <Retro::Ranges::UnrealSizedString R>
constexpr auto size(const R &Range)
{
    return Range.Len();
}

template <Retro::Ranges::CanBridgeToRange I>
constexpr auto begin(I &Range)
{
    return Retro::Ranges::TAdapterIterator<Retro::Ranges::TIteratorType<I>, Retro::Ranges::TSentinelType<I>>(
        Range.begin());
}

template <Retro::Ranges::CanBridgeToRange I>
constexpr auto end(I &Range)
{
    return Retro::Ranges::TSentinelAdapter<Retro::Ranges::TIteratorType<I>, Retro::Ranges::TSentinelType<I>>(
        Range.end());
}

#if !UE_BUILD_SHIPPING
template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<TArray<T, A>>().begin())>)
constexpr auto begin(TArray<T, A> &Array)
{
    return Retro::Ranges::TArrayIterator<TArray<T, A>, T>(Array.GetData(), Array);
}

template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<const TArray<T, A>>().begin())>)
constexpr auto begin(const TArray<T, A> &Array)
{
    return Retro::Ranges::TArrayIterator<const TArray<T, A>, const T>(Array.GetData(), Array);
}

template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<TArray<T, A>>().end())>)
constexpr auto end(TArray<T, A> &Array)
{
    return Retro::Ranges::TArrayIterator<TArray<T, A>, T>(Array.GetData() + Array.Num(), Array);
}

template <typename T, typename A>
    requires(!std::input_iterator<decltype(std::declval<const TArray<T, A>>().end())>)
constexpr auto end(const TArray<T, A> &Array)
{
    return Retro::Ranges::TArrayIterator<const TArray<T, A>, const T>(Array.GetData() + Array.Num(), Array);
}

constexpr auto begin(FString &String)
{
    if constexpr (std::contiguous_iterator<decltype(String.begin())>)
    {
        return String.begin();
    }
    else
    {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData(), String);
    }
}

constexpr auto begin(const FString &String)
{
    if constexpr (std::contiguous_iterator<decltype(String.begin())>)
    {
        return String.begin();
    }
    else
    {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData(), String);
    }
}

constexpr auto end(FString &String)
{
    if constexpr (std::contiguous_iterator<decltype(String.end())>)
    {
        return String.end();
    }
    else
    {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData() + String.Len(), String);
    }
}

constexpr auto end(const FString &String)
{
    if constexpr (std::contiguous_iterator<decltype(String.end())>)
    {
        return String.end();
    }
    else
    {
        return Retro::Ranges::TArrayIterator(String.GetCharArray().GetData() + String.Len(), String);
    }
}
#endif

template <typename T, typename A>
constexpr T *data(TArray<T, A> &Array)
{
    return Array.GetData();
}

template <typename T, typename A>
constexpr const T *data(const TArray<T, A> &Array)
{
    return Array.GetData();
}

template <typename T, typename A>
constexpr T *data(TArray<T, A> &&Array)
{
    return std::move(Array).GetData();
}

inline TCHAR *data(FString &String)
{
    return String.GetCharArray().GetData();
}

inline const TCHAR *data(const FString &String)
{
    return String.GetCharArray().GetData();
}

inline TCHAR *data(FString &&String)
{
    return std::move(String).GetCharArray().GetData();
}