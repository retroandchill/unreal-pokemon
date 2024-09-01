// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Concepts/UEContainer.h"
#include <TypeTraits.h>

/**
 * Functionality pertaining to UE support for C++ 20 Ranges/range-v3.
 */
namespace UE::Ranges {

    template <typename T>
    concept UnrealInterface = !std::is_base_of_v<UObject, T> && requires { typename T::UClassType; };

    /**
     * Concept to check if the passed in functor is a valid functional type.
     * @tparam T The type to check against
     */
    template <typename T>
    concept FunctionalType = requires { typename StdExt::FunctionType_t<T>; };

    template <typename P, typename T>
    concept PointerTo = requires(P &&Ptr, T &&Value) {
        { *Ptr } -> std::convertible_to<std::add_lvalue_reference_t<T>>;
    };

    template <typename T>
    concept MapPair = requires(T&& Pair) {
        Pair.Key;
        Pair.Value;
    };

    
} // namespace UE::Ranges