// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "RetroLib/Ranges/Views/NameAliases.h"

#ifndef RETROLIB_EXPORT
#define RETROLIB_EXPORT
#endif

namespace Retro {
    /**
     * Custom iterator type used to cycle through a sized contiguous range.
     * 
     * @tparam T The type of data being stored
     */
    RETROLIB_EXPORT template <typename T>
    class TCircularIterator {
        using DifferenceType = std::ranges::range_difference_t<TSpan<T>>;
    
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
        template <typename R>
            requires std::ranges::contiguous_range<R> && std::ranges::sized_range<R> && std::common_reference_with<T&, std::ranges::range_reference_t<R>>
        explicit TCircularIterator(R&& DataIn, int32 InitialIndex = 0)
            : Data(std::forward<R>(DataIn)), CurrentIndex(InitialIndex) {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
        }

        /**
         * Dereference the data at the current index.
         * @return A reference to the current index
         */
        const T &operator*() const {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
            return Data[CurrentIndex];
        }

        /**
         * Dereference the data at the current index.
         * @return A pointer to the current index
         */
        const T *operator->() const {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
            return &Data[CurrentIndex];
        }

        /**
         * Increment the iterator by the given number of poisitions, returning to the beginning if the end has been reached.
         * @param Amount The number of positions to increment by
         * @return A reference to the incremented iterator
         */
        TCircularIterator &operator+=(DifferenceType Amount) {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
            CurrentIndex = (CurrentIndex + Data.size() + Amount % Data.size()) % Data.size();
            return *this;
        }

        /**
         * Increment the iterator by 1 position, returning to the beginning if the end has been reached.
         * @return A reference to the incremented iterator
         */
        TCircularIterator &operator++() {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
            if (CurrentIndex == Data.size() - 1) {
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
        TCircularIterator &operator-=(DifferenceType Amount) {
            return *this += -Amount;
        }

        /**
         * Decrement the iterator by 1 position, returning to the end if the beginning has been reached.
         * @return A reference to the incremented iterator
         */
        TCircularIterator &operator--() {
            check(Data.size() > CurrentIndex && CurrentIndex >= 0)
            if (CurrentIndex == 0) {
                CurrentIndex = Data.size() - 1;
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

        int32 GetIndex() const {
            return CurrentIndex;
        }

        void SetIndex(int32 Index) {
            check(Data.size() > Index && Index >= 0)
            CurrentIndex = Index;
        }

        /**
         * Return if the iterator in question is valid or not.
         * @return Is the container referenced non-empty.
         */
        bool IsValid() const {
            return Data.size() > 0;
        }

        /**
         * Check if there are enough elements for this iterator to cycle.
         * @return Can this iterator cycle
         */
        bool CanCycle() const {
            return Data.size() > 1;
        }

    private:
        /**
         * A reference to the contained data.
         */
        Retro::TSpan<T> Data;

        /**
         * The current index of the iterator.
         */
        int32 CurrentIndex = 0;
    };
}