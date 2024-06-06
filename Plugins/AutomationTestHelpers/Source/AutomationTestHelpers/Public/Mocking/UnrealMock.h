// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "External/fakeit.hpp"
#include "InterfaceShim.h"
#include "MockedObject.h"

namespace UnrealMock {

/**
 * Mock an Unreal Engine interface and create a script interface to use with it.
 * @tparam T The interface type
 * @param Outer The owning object for the created mock
 * @return The created script interface object (for passing to methods) and the actual mock (for mocking the methods of)
 */
template <typename T>
std::pair<TScriptInterface<T>, fakeit::Mock<TInterfaceShim<T>>> CreateMock(UObject *Outer = GetTransientPackage()) {
    std::pair<TScriptInterface<T>, fakeit::Mock<TInterfaceShim<T>>> Ret;
    Ret.first = NewObject<UMockedObject>(Outer);
    Ret.first.SetInterface(&Ret.second.get());
    return Ret;
}

} // namespace UnrealMock