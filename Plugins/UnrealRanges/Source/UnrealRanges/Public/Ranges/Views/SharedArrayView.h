// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Views/ContainerView.h"

namespace UE::Ranges {
    template <typename T>
    class TSharedArrayIterator {
    public:
        using SizeType = typename TArray<T>::SizeType;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        
        TSharedArrayIterator() = default;

        explicit TSharedArrayIterator(const TSharedRef<TArray<T>>& Data, SizeType Index = 0) : Data(Data), Index(Index) {}
        
        bool operator==(const TSharedArrayIterator& Other) const {
            check(Data == Other.Data)
            return Index == Other.Index;
        }
        
        T& operator * () const {
            check(Data.IsValid() && Data->IsValidIndex(Index))
            return (*Data)[Index];
        }
        
        TSharedArrayIterator& operator++() {
            ++Index;
            return *this;
        }
        TSharedArrayIterator operator++(int) {
            TSharedArrayIterator Temp = *this;
            ++*this;
            return Temp;
        }

    private:
        TSharedPtr<TArray<T>> Data;
        SizeType Index = INDEX_NONE;
    };

    /**
     * @class TSharedArrayView
     *
     * @brief A template class for creating a shared view of an array-like container.
     *
     * TSharedArrayView provides a way to manage and access an array-like data structure
     * without taking ownership of the underlying data. This can be useful for
     * efficiently sharing and passing around large datasets without unnecessary copying.
     *
     * @tparam T Type of the elements in the array view.
     */
    template <typename T>
    class TSharedArrayView {
    public:
        /**
         * @brief Default constructor for the TSharedArrayView class.
         *
         * This default constructor initializes an empty shared array view.
         */
        explicit TSharedArrayView() = default;

        /**
         * @brief Constructs a TSharedArrayView from an rvalue reference to an array.
         *
         * This constructor takes ownership of the provided array by moving its contents
         * into a shared array view. The original array will be left in a valid but unspecified state.
         *
         * @param Array An rvalue reference to the array that will be used to initialize the view.
         *              The contents of the array will be moved and managed by the TSharedArrayView.
         * @return An instance of TSharedArrayView that holds a shared reference to the array.
         */
        explicit TSharedArrayView(TArray<T>&& Array) : Data(MakeShared<TArray<T>>(std::move(Array))) {}

        /**
         * @brief Returns an iterator to the beginning of the shared array view.
         *
         * This method provides a constant iterator pointing to the first element of the
         * array managed by the shared array view.
         *
         * @return An iterator to the beginning of the shared array.
         */
        TSharedArrayIterator<T> begin() const {
            return TSharedArrayIterator<T>(Data);
        }

        /**
         * @brief Checks whether the end of the input has been reached.
         *
         * This method determines if the parser has reached the end of the input stream.
         * It can be used to verify if there is no more data to be processed.
         *
         * @return Returns true if the end of the input is reached, otherwise false.
         */
        TSharedArrayIterator<T> end() const {
            return TSharedArrayIterator<T>(Data, Data->Num());
        }

    private:
        TSharedRef<TArray<T>> Data = MakeShared<TArray<T>>();
    };

    static_assert(ranges::forward_range<TSharedArrayView<UObject>>);
}

template <typename T>
constexpr inline bool ranges::enable_view<UE::Ranges::TSharedArrayView<T>> = true;

static_assert(ranges::viewable_range<UE::Ranges::TSharedArrayView<int32>>);

