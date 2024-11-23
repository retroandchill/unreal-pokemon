// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "ForwardLike.h"
#include "Ranges/Concepts/Tuples.h"

namespace UE::Ranges {
    /**
     * @class TGetFromTuple
     * @brief Utility class for extracting elements from a std::tuple.
     *
     * This class provides functionality to retrieve and manipulate
     * elements within a std::tuple, enabling access to elements
     * stored at specific positions in a type-safe manner.
     *
     * @tparam N The zero-based index of the element in the tuple to be accessed.
     */
    template <size_t N>
    struct TGetFromTuple {
        template <typename T>
            requires TupleLike<T> && HasTupleElement<T, N>
        constexpr decltype(auto) operator()(T &&Tuple) const {
            return get<N>(std::forward<T>(Tuple));
        }
    };

    /**
     * @brief Utility variable for extracting elements from a std::tuple.
     *
     * This variable is an instance of TGetFromTuple, providing functionality
     * to access elements at a specific position in a tuple in a type-safe manner.
     *
     * @tparam N The zero-based index of the element in the tuple to be accessed.
     */
    template <size_t N>
    constexpr TGetFromTuple<N> GetFromTuple;
} // namespace UE::Ranges
