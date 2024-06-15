// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Shorthand for a single argument callback that takes in an interface
 */
template <typename T>
using TInterfaceCallback = const TFunctionRef<void(const TScriptInterface<T>&)>&;