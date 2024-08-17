// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include <range/v3/range/concepts.hpp>

/**
 * Functionality pertaining to UE support for C++ 20 Ranges/range-v3.
 */
namespace UE::Ranges {

template <typename>
struct TIsUEContainer : std::false_type {};

template <typename T, typename A>
struct TIsUEContainer<TArray<T, A>> : std::true_type {};

template <typename T, typename K, typename A>
struct TIsUEContainer<TSet<T, K, A>> : std::true_type {};

template <typename K, typename V, typename A, typename F>
struct TIsUEContainer<TMap<K, V, A, F>> : std::true_type {};

template <typename T>
concept IsUEContainer = TIsUEContainer<std::remove_cvref_t<T>>::value;

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
FNo operator==(const T&, const U&);

/**
 * Struct used to check if equality between values exists.
 * @tparam T The type of the LHS.
 * @tparam U The type of the RHS. 
 */
template <typename T, typename U = T>
struct TEqualExists {
    enum {
        Value = !std::is_same_v<decltype(std::declval<T>() == std::declval<U>()), FNo>
    };
};

}