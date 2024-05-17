// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Custom iterator type used to cycle through an array.
 * @tparam T The type of data being stored
 */
template <typename T>
class TCircularIterator {
  public:
    /**
     * Create an empty iterator.
     */
    explicit TCircularIterator() = default;

    /**
     * Construct a new iterator object from the given data and initial index.
     * @param DataIn The input data to manage
     * @param InitialIndex The initial index to set the iterator to
     */
    explicit TCircularIterator(TConstArrayView<T> DataIn, int32 InitialIndex = 0)
        : Data(DataIn), CurrentIndex(InitialIndex) {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
    }

    /**
     * Dereference the data at the current index.
     * @return A reference to the current index
     */
    const T &operator*() const {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
        return Data[CurrentIndex];
    }

    /**
     * Dereference the data at the current index.
     * @return A pointer to the current index
     */
    const T *operator->() const {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
        return &Data[CurrentIndex];
    }

    /**
     * Increment the iterator by the given number of poisitions, returning to the beginning if the end has been reached.
     * @param Amount The number of positions to increment by
     * @return A reference to the incremented iterator
     */
    TCircularIterator &operator+=(int32 Amount) {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
        CurrentIndex = (CurrentIndex + Data.Num() + Amount % Data.Num()) % Data.Num();
        return *this;
    }

    /**
     * Increment the iterator by 1 position, returning to the beginning if the end has been reached.
     * @return A reference to the incremented iterator
     */
    TCircularIterator &operator++() {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
        if (CurrentIndex == Data.Num() - 1) {
            CurrentIndex = 0;
        } else {
            CurrentIndex++;
        }

        return *this;
    }

    /**
     * Increment the iterator by 1 position, returning to the beginning if the end has been reached.
     * @return A reference to the incremented iterator
     */
    [[nodiscard]] TCircularIterator operator++(int) {
        auto Temp = *this;
        ++*this;
        return Temp;
    }

    /**
     * Increment the iterator by the given number of positions, returning to the beginning if the end has been reached.
     * @param Amount The number of positions to increment by
     * @return A reference to the incremented iterator
     */
    TCircularIterator &operator-=(int32 Amount) {
        return *this += -Amount;
    }

    /**
     * Decrement the iterator by 1 position, returning to the end if the beginning has been reached.
     * @return A reference to the incremented iterator
     */
    TCircularIterator &operator--() {
        check(Data.Num() > CurrentIndex && CurrentIndex >= 0)
        if (CurrentIndex == 0) {
            CurrentIndex = Data.Num() - 1;
        } else {
            CurrentIndex--;
        }

        return *this;
    }

    /**
     * Decrement the iterator by 1 position, returning to the end if the beginning has been reached.
     * @return A reference to the incremented iterator
     */
    [[nodiscard]] TCircularIterator operator--(int) {
        auto Temp = *this;
        --*this;
        return Temp;
    }

    /**
     * Return if the iterator in question is valid or not.
     * @return Is the container referenced non-empty.
     */
    bool IsValid() const {
        return Data.Num() > 0;
    }

    /**
     * Check if there are enough elements for this iterator to cycle.
     * @return Can this iterator cycle
     */
    bool CanCycle() const {
        return Data.Num() > 1;
    }

  private:
    /**
     * A reference to the contained data.
     */
    TConstArrayView<T> Data;

    /**
     * The current index of the iterator.
     */
    int32 CurrentIndex = 0;
};
