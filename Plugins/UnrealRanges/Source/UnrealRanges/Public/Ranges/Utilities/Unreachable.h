// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    /**
     * Marks a code path as unreachable and informs the compiler optimizer of this fact.
     * This can be useful to improve optimization and remove dead code paths.
     *
     * The function uses different compiler-specific intrinsics:
     * - For MSVC: `__assume(false);`
     * - For GCC and Clang: `__builtin_unreachable();`
     *
     * The attribute `[[noreturn]]` indicates that the function will not return control to the caller.
     */
    [[noreturn]] inline void Unreachable() {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
#else // GCC, Clang
        __builtin_unreachable();
#endif
    }
} // namespace UE::Ranges
