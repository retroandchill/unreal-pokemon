﻿// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Ranges/Concepts/HasFunctionCallOperator.h"

/**
 * Functionality pertaining to UE support for C++ 20 Ranges/range-v3.
 */
namespace UE::Ranges {

    /**
     * Used to check if the given type is an Unreal Interface
     * @param T The particular interface type
     */
    template <typename T>
    concept UnrealInterface = !std::is_base_of_v<UObject, T> && std::same_as<typename T::ThisClass, T> &&
                              std::is_base_of_v<UInterface, typename T::UClassType> && requires(T &&Interface) {
                                  { Interface._getUObject() } -> std::same_as<UObject *>;
                              };

    /**
     * Concept to check if the passed in functor is a valid functional type.
     * @tparam T The type to check against
     */
    template <typename T>
    concept FunctionalType = std::is_function_v<std::remove_pointer_t<T>> || HasFunctionCallOperator<T>;

    template <typename P, typename T>
    concept PointerTo = requires(P &&Ptr, T &&Value) {
        { *Ptr } -> std::convertible_to<std::add_lvalue_reference_t<T>>;
    };

    template <typename T>
    concept MapPair = requires(T &&Pair) {
        Pair.Key;
        Pair.Value;
    };

} // namespace UE::Ranges