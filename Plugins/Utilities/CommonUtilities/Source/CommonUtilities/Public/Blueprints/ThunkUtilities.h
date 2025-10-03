// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "Unreachable.h"

#define P_GET_OPAQUE_STRUCT(PropVar, PointerVar)                                                                       \
    Stack.StepCompiledIn<FStructProperty>(nullptr);                                                                    \
    const auto *PropVar = CastField<FStructProperty>(Stack.MostRecentProperty);                                        \
    auto *PointerVar = Stack.MostRecentPropertyAddress;

#define P_GET_RESULT(Type, Name) auto &Name = *static_cast<Type *>(RESULT_PARAM)

#define CUSTOM_THUNK_STUB_CONST(RetType, Method, ...)                                                                  \
    RetType Method(__VA_ARGS__) const                                                                                  \
    {                                                                                                                  \
        check(false);                                                                                                  \
        UE::Unreachable();                                                                                             \
    }

#define CUSTOM_THUNK_STUB(RetType, Method, ...)                                                                        \
    RetType Method(__VA_ARGS__)                                                                                        \
    {                                                                                                                  \
        check(false);                                                                                                  \
        UE::Unreachable();                                                                                             \
    }