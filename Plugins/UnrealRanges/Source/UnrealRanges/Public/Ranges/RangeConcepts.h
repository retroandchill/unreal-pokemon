// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Concepts/UEContainer.h"
#include <range/v3/range/concepts.hpp>

/**
 * Functionality pertaining to UE support for C++ 20 Ranges/range-v3.
 */
namespace UE::Ranges {

    /**
     * Internal namespace for handling details for the concepts of the UE ranges library.
     */
    namespace Detail {
        /**
         * Filler struct to indicate no existing operator.
         */
        struct FNo {};

        /**
         * Default implementation for the == operator used for type checks.
         * @tparam T The type of the LHS.
         * @tparam U The type of the RHS.
         * @return No existing overload exists.
         */
        template <typename T, typename U>
        FNo operator==(const T &, const U &);

        /**
         * Struct used to check if equality between values exists.
         * @tparam T The type of the LHS.
         * @tparam U The type of the RHS.
         */
        template <typename T, typename U = T>
        struct TEqualExists {
            static constexpr bool Value = !std::is_same_v<decltype(std::declval<T>() == std::declval<U>()), FNo>;
        };

    } // namespace Detail

    /**
     * Concept to check if two types have the == operator overloaded or not?
     * @tparam T The type of the LHS.
     * @tparam U The type of the RHS.
     */
    template <typename T, typename U = T>
    concept EqualExists = Detail::TEqualExists<T, U>::Value;

} // namespace UE::Ranges