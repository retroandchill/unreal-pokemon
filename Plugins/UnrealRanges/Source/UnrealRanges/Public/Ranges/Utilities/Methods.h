// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define ABSTRACT_METHOD { LowLevelFatalError(TEXT("Pure virtual not implemented (%s)"), TEXT(__PRETTY_FUNCTION__)); std::unreachable(); }