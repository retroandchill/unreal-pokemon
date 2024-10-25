// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

namespace UE::Ranges {
    [[noreturn]] inline void Unreachable() {
#if defined(_MSC_VER) && !defined(__clang__) // MSVC
        __assume(false);
#else // GCC, Clang
        __builtin_unreachable();
#endif
    }
}

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ABSTRACT_METHOD { LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(__PRETTY_FUNCTION__)); UE::Ranges::Unreachable(); }