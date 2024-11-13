// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Definition of C++23's std::forward_like for usage in C++20. This is slated to be removed once Unreal adds C++23 support.
     * 
     * @tparam T The type we're forwarding like
     * @tparam U The type of the parameter
     * @param Value The value to forward
     * @return The forwarded value
     */
    template <typename T, typename U>
    constexpr decltype(auto) ForwardLike(U&& Value) {
        constexpr bool bIsAddingConst = std::is_const_v<std::remove_reference_t<T>>;
        if constexpr (std::is_lvalue_reference_v<T&&>) {
            if constexpr (bIsAddingConst) {
                return std::as_const(Value);
            } else {
                return static_cast<U&>(Value);
            }
        } else {
            if constexpr (bIsAddingConst) {
                return std::move(std::as_const(Value));
            } else {
                return std::move(Value);
            }
        }
    }
}
