// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Shorthand for a single argument callback that takes in an interface
 * @tparam T The interface type we're looking for
 */
template <typename T>
using TInterfaceCallback = const TFunctionRef<void(const TScriptInterface<T> &)> &;