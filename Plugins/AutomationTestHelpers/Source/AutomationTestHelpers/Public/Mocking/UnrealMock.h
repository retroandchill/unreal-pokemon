// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "MockedObject.h"

namespace UnrealMock {

template <typename T>
TScriptInterface<T> CreateMockedWrapper(T& Mock, UObject *Outer = GetTransientPackage()) {
    TScriptInterface<T> Ret = NewObject<UMockedObject>(Outer);
    Ret.SetInterface(&Mock);
    return Ret;
}

} // namespace UnrealMock

#define CREATE_MOCK(InterfaceType, InterfaceName, MockType, MockName) \
    MockType MockName; \
    auto InterfaceName = UnrealMock::CreateMockedWrapper<InterfaceType>(MockName)