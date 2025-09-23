// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/UObjectIterator.h"

template <typename T>
concept UnrealIterator = requires(const T &Iterator) {
    requires !std::is_void_v<decltype(*Iterator)>;
    *Iterator;
    { !Iterator } -> std::convertible_to<bool>;
} && requires(T &Iterator) { ++Iterator; };

template <UnrealIterator T>
using TUnrealIteratorValue = std::decay_t<decltype(*std::declval<T>())>;

static_assert(UnrealIterator<TObjectIterator<UObject>>);

template <UnrealIterator T>
class TUnrealIteratorAdapterStorage
{
  public:
    template <typename... A>
        requires std::is_constructible_v<T, A...>
    explicit TUnrealIteratorAdapterStorage(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }

    bool operator!() const
    {
        return !Iterator;
    }

    auto operator*() const
    {
        return *Iterator;
    }

    void operator++()
    {
        ++Iterator;
    }

  private:
    T Iterator;
};

template <UnrealIterator T>
    requires(!std::is_default_constructible_v<T>)
class TUnrealIteratorAdapterStorage<T>
{
  public:
    TUnrealIteratorAdapterStorage() = default;

    template <typename... A>
        requires std::is_constructible_v<T, A...>
    explicit TUnrealIteratorAdapterStorage(A &&...Args) : Iterator(InPlace, Forward<A>(Args)...)
    {
    }

    bool operator!() const
    {
        return !*Iterator;
    }

    auto operator*() const
    {
        return **Iterator;
    }

    void operator++()
    {
        ++*Iterator;
    }

  private:
    TOptional<T> Iterator;
};

template <UnrealIterator T>
class TUnrealIteratorAdapter
{
  public:
    using difference_type = std::ptrdiff_t;
    using value_type = TUnrealIteratorValue<T>;

    template <typename... A>
        requires std::is_constructible_v<TUnrealIteratorAdapterStorage<T>, A...>
    explicit TUnrealIteratorAdapter(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }
    bool operator==(const std::default_sentinel_t &) const
    {
        return !Iterator;
    }

    decltype(auto) operator*()
    {
        return *Iterator;
    }

    decltype(auto) operator*() const
    {
        return *Iterator;
    }

    TUnrealIteratorAdapter &operator++()
    {
        ++Iterator;
        return *this;
    }

    void operator++(int)
    {
        ++Iterator;
    }

  private:
    TUnrealIteratorAdapterStorage<T> Iterator;
};

template <UnrealIterator T>
class TUnrealIteratorRange
{
  public:
    template <typename... A>
        requires std::constructible_from<T, A...>
    explicit TUnrealIteratorRange(A &&...Args) : Iterator(Forward<A>(Args)...)
    {
    }

    TUnrealIteratorAdapter<T> &begin() &
    {
        return Iterator;
    }

    TUnrealIteratorAdapter<T> begin() const &
    {
        return Iterator;
    }

    TUnrealIteratorAdapter<T> &&begin() &&
    {
        return MoveTemp(Iterator);
    }

    std::default_sentinel_t end()
    {
        return {};
    }

  private:
    TUnrealIteratorAdapter<T> Iterator;
};

template <typename T>
    requires std::derived_from<T, FField> || std::derived_from<T, UField>
using TFieldView = TUnrealIteratorRange<TFieldIterator<T>>;

template <std::derived_from<UObject> T>
using TObjectView = TUnrealIteratorRange<TObjectIterator<T>>;

namespace ranges
{
    template <UnrealIterator T>
    constexpr bool enable_borrowed_range<TUnrealIteratorRange<T>> = true;
}