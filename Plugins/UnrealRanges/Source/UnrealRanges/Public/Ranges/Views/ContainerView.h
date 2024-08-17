﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include <range/v3/range/concepts.hpp>

namespace UE::Ranges {

/**
 * Generic iterator that wraps around any normal iterator.
 * @tparam I The underlying iterator type.
 */
template <typename I>
class TViewIterator {

public:
    /**
     * The type that dereferencing the iterator returns.
     */
    using FIteratorType = decltype(*std::declval<I>());

    /**
     * The value used to determine the difference type, needed to satisfy the ranges concept.
     */
    using difference_type = std::ptrdiff_t;

    /**
     * The raw non-const non-referenced value type. Used to satisfy the ranges concepts.
     */
    using value_type = std::remove_const_t<std::remove_reference_t<FIteratorType>>;

    /**
     * Default constructor.
     */
    TViewIterator() = default;

    /**
     * Constructor that takes in an iterator object.
     * @param Iterator The base iterator that is taken in by this type.
     */
    explicit TViewIterator(const I &Iterator) : Iterator(Iterator) {
    }

    /**
     * Equality operator with another iterator.
     * @param Other The other iterator in question.
     * @return Are the iterators equal.
     */
    FORCEINLINE bool operator==(const TViewIterator &Other) const {
        if constexpr (TEqualExists<I>::Value) {
            return Iterator == Other.Iterator;
        } else {
            if (Iterator.IsSet() && Other.Iterator.IsSet()) {
                return &*Iterator.GetValue() == &*Other.Iterator.GetValue();
            }

            return Iterator.IsSet() == Other.Iterator.IsSet();
        }
    }

    /**
     * Deference operator for the iterator.
     * @return The dereferenced iterator value.
     */
    FORCEINLINE FIteratorType operator*() const {
        return **Iterator;
    }

    /**
     * Prefix increment operator.
     * @return A reference to the iterator after the increment. 
     */
    TViewIterator &operator++() {
        ++*Iterator;
        return *this;
    }

    /**
     * Post-fix increment operator.
     * @return The state of the iterator before the increment.
     */
    TViewIterator operator++(int) {
        auto Temp = *this;
        ++*Iterator;
        return Temp;
    }

private:
    TOptional<I> Iterator;

};

static_assert(ranges::forward_iterator<TViewIterator<TArray<int32>::RangedForConstIteratorType>>);

/**
 * Generic view declaration used to hold a copy of a container.
 * @tparam T The container that is held.
 */
template <typename T>
    requires IsUEContainer<T>
class TUEContainerView {
public:
    /**
     * The type of the standard iterator.
     */
    using FIterator = TViewIterator<decltype(std::declval<T>().begin())>;

    /**
     * The type of the const iterator.
     */
    using FConstIterator = TViewIterator<decltype(std::declval<const T>().begin())>;

    /**
     * The type for the size of the container.
     */
    using FSizeType = decltype(std::declval<T>().Num());

    /**
     * Construct a new view with a reference to a container.
     * @param Container The container to reference.
     */
    explicit TUEContainerView(T &Container) requires std::is_reference_v<T> : Container(Container) {
    }

    /**
     * Construct a new view moving in a container.
     * @param Container The container to move in.
     */
    explicit TUEContainerView(T &&Container) requires (!std::is_reference_v<T>) : Container(MoveTemp(Container)) {
    }

    /**
     * Iteration begin operation.
     * @return The iterator for the first element.
     */
    FORCEINLINE FIterator begin() {
        return FIterator(Container.begin());
    }

    /**
     * Iteration end operation.
     * @return The iterator for the termination of iteration.
     */
    FORCEINLINE FIterator end() {
        return FIterator(Container.end());
    }

    /**
     * Iteration begin operation.
     * @return The iterator for the first element.
     */
    FORCEINLINE FConstIterator begin() const {
        return FConstIterator(Container.begin());
    }

    /**
     * Iteration end operation.
     * @return The iterator for the termination of iteration.
     */
    FORCEINLINE FConstIterator end() const {
        return FConstIterator(Container.end());
    }

    /**
     * Size method to satisfy the sized range concept.
     * @return The size of the view.
     */
    FORCEINLINE FSizeType size() const {
        return Container.Num();
    }

private:
    T Container;
};

static_assert(ranges::forward_range<TUEContainerView<TArray<int32>>>);
static_assert(ranges::sized_range<TUEContainerView<TArray<int32>>>);
static_assert(ranges::forward_range<TUEContainerView<TArray<int32>&>>);
static_assert(ranges::sized_range<TUEContainerView<TArray<int32>&>>);
static_assert(ranges::forward_range<TUEContainerView<const TArray<int32>&>>);
static_assert(ranges::sized_range<TUEContainerView<const TArray<int32>&>>);

}