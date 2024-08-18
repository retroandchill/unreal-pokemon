// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/RangeConcepts.h"
#include <range/v3/range/concepts.hpp>
#include <range/v3/view/view.hpp>

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
        if constexpr (EqualExists<I>) {
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
 * Generic view declaration used to contain a non-owing view of an unreal engine container.
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
    explicit constexpr TUEContainerView(T &Container) : Container(&Container) {
    }

    /**
     * Iteration begin operation.
     * @return The iterator for the first element.
     */
    FORCEINLINE FIterator begin() {
        return FIterator(Container->begin());
    }

    /**
     * Iteration end operation.
     * @return The iterator for the termination of iteration.
     */
    FORCEINLINE FIterator end() {
        return FIterator(Container->end());
    }

    /**
     * Iteration begin operation.
     * @return The iterator for the first element.
     */
    FORCEINLINE FConstIterator begin() const {
        return FConstIterator(const_cast<const T*>(Container)->begin());
    }

    /**
     * Iteration end operation.
     * @return The iterator for the termination of iteration.
     */
    FORCEINLINE FConstIterator end() const {
        return FConstIterator(const_cast<const T*>(Container)->end());
    }

    /**
     * Size method to satisfy the sized range concept.
     * @return The size of the view.
     */
    FORCEINLINE FSizeType size() const {
        return Container->Num();
    }

private:
    T *Container = nullptr;
};

/**
 * Create a new range from the provided array.
 * @tparam T The type of data the array holds
 * @param Container The array view to create the view from.
 * @return The created view
 */
template <typename T>
    requires IsUEContainer<T>
FORCEINLINE constexpr auto CreateRange(T& Container) {
    return TUEContainerView<T>(Container);
}

}

/**
 * Overload of the pipe operator so that UE containers can be piped directly into a range without calling
 * CreateRange on it first.
 * @tparam T The type of the container
 * @tparam V The type of the view closure being passed in
 * @param Container The container being passed in
 * @param ViewClosure The closure to pipe into
 * @return The result of the view operation
 */
template <typename T, typename V>
    requires UE::Ranges::IsUEContainer<T>
FORCEINLINE constexpr auto operator|(T& Container, ranges::views::view_closure<V> ViewClosure) {
    return UE::Ranges::CreateRange(Container) | ViewClosure;
}

/**
 * Template specialization to ensure that TUEContainerView is considered a view by the range system.
 * @tparam T The type of the container that is being viewed
 */
template <typename T>
    requires UE::Ranges::IsUEContainer<T>
constexpr inline bool ranges::enable_view<UE::Ranges::TUEContainerView<T>> = true;

static_assert(ranges::forward_range<UE::Ranges::TUEContainerView<TArray<int32>>>);
static_assert(ranges::sized_range<UE::Ranges::TUEContainerView<TArray<int32>>>);
static_assert(ranges::viewable_range<UE::Ranges::TUEContainerView<TArray<int32>>>);