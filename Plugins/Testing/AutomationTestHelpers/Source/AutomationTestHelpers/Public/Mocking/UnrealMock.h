// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "MockedObject.h"

namespace UnrealMock
{

    template <typename T>
    TScriptInterface<T> CreateMockedWrapper(T &Mock, UObject *Outer = GetTransientPackage())
    {
        TScriptInterface<T> Ret = NewObject<UMockedObject>(Outer);
        Ret.SetInterface(&Mock);
        return Ret;
    }

    template <typename T>
    TScriptInterface<T> CreateMockedActor(T &Mock, UWorld *World)
    {
        auto Actor = World->SpawnActor<AActor>();
        TScriptInterface<T> Ret = Actor;
        Ret.SetInterface(&Mock);
        return Ret;
    }

} // namespace UnrealMock

#define DECLARE_MOCK_INTERFACE(ClassName)                                                                              \
  private:                                                                                                             \
    TObjectPtr<UObject> Object;                                                                                        \
                                                                                                                       \
  public:                                                                                                              \
    ClassName() = default;                                                                                             \
    ~ClassName() override = default;                                                                                   \
    UObject *_getUObject() const override                                                                              \
    {                                                                                                                  \
        return Object;                                                                                                 \
    }                                                                                                                  \
    void SetObject(UObject *NewObject)                                                                                 \
    {                                                                                                                  \
        Object = NewObject;                                                                                            \
    }                                                                                                                  \
                                                                                                                       \
  private:

#define CREATE_MOCK(InterfaceType, InterfaceName, MockType, MockName)                                                  \
    MockType MockName;                                                                                                 \
    auto InterfaceName = UnrealMock::CreateMockedWrapper<InterfaceType>(MockName);                                     \
    MockName.SetObject(InterfaceName.GetObject())

#define CREATE_MOCK_ACTOR(World, InterfaceType, InterfaceName, MockType, MockName)                                     \
    MockType MockName;                                                                                                 \
    auto InterfaceName = UnrealMock::CreateMockedActor<InterfaceType>(MockName, World);                                \
    MockName.SetObject(InterfaceName.GetObject())