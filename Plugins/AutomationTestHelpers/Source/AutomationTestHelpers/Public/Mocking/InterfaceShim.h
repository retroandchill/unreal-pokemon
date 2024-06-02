// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"

/**
 * Basic shim for an Unreal Engine interface, used to make the protected virtual destructor public.
 */
template <typename T>
class TInterfaceShim : public T {
public:
    ~TInterfaceShim() override = default;
};
