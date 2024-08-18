// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
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
 * Type trait to signify if a container is a UE container of some kind.
 */
template <typename>
struct TIsUEContainer : std::false_type {};

/**
 * Template specialization for TArray so that a range can be made from a reference to one.
 * @tparam T The type of data held by the array.
 * @tparam A The allocator used for the array
 */
template <typename T, typename A>
struct TIsUEContainer<TArray<T, A>> : std::true_type {};

/**
 * Template specialization for TArrayView so that a range can be made from a reference to one.
 * @tparam T The type of data held by the array.
 * @tparam S The size type of the array being viewed.
 */
template <typename T, typename S>
struct TIsUEContainer<TArrayView<T, S>> : std::true_type {};

/**
 * Template specialization for TSet so that a range can be made from a reference to one.
 * @tparam T The type of data held by the set.
 * @tparam K The type of the key function.
 * @tparam A The type of the allocator for the set
 */
template <typename T, typename K, typename A>
struct TIsUEContainer<TSet<T, K, A>> : std::true_type {};

/**
 * Template specialization for TMap so that a range can be made from a reference to one.
 * @tparam K The type of the map's keys
 * @tparam V The type of the map's values
 * @tparam A The type of the allocator for the map
 * @tparam F The type of the key function.
 */
template <typename K, typename V, typename A, typename F>
struct TIsUEContainer<TMap<K, V, A, F>> : std::true_type {};

}

/**
 * Concept used to check if the passed type is a valid UE container.
 * @tparam T The type to check against.
 */
template <typename T>
concept IsUEContainer = Detail::TIsUEContainer<std::remove_cvref_t<T>>::value;

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
FNo operator==(const T&, const U&);

/**
 * Struct used to check if equality between values exists.
 * @tparam T The type of the LHS.
 * @tparam U The type of the RHS. 
 */
template <typename T, typename U = T>
struct TEqualExists {
    static constexpr bool Value = !std::is_same_v<decltype(std::declval<T>() == std::declval<U>()), FNo>;
};

}

/**
 * Concept to check if two types have the == operator overloaded or not?
 * @tparam T The type of the LHS.
 * @tparam U The type of the RHS. 
 */
template <typename T, typename U = T>
concept EqualExists = Detail::TEqualExists<T, U>::Value;

}