// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Optionals/AndThen.h"
#include "RetroLib/Optionals/Transform.h"

namespace Retro {
    FVariantObjectStructRegistry &FVariantObjectStructRegistry::Get() {
        static FVariantObjectStructRegistry Instance;
        return Instance;
    }

    TOptional<IVariantRegistration &> FVariantObjectStructRegistry::GetVariantStructData(const UScriptStruct &Struct) {
        return Optionals::OfNullable(RegisteredStructs.Find(Struct.GetFName())) |
               Optionals::Transform(&TSharedRef<IVariantRegistration>::Get);
    }

    TOptional<IVariantConversion &> FVariantObjectStructRegistry::GetVariantStructConversion(const UScriptStruct &From,
                                                                                             const UScriptStruct &To) {
        return GetVariantStructData(From) |
               Optionals::AndThen(BindBack<&IVariantRegistration::GetConversion>(std::ref(To)));
    }

#if RETROLIB_WITH_UE5CORO
    UE5Coro::TCoroutine<IVariantRegistration *>
    FVariantObjectStructRegistry::AsyncGetVariantStructData(const UScriptStruct &Struct) {
        if (auto StructData = RegisteredStructs.Find(Struct.GetFName()); StructData != nullptr) {
            co_return &StructData->Get();
        }

        while (true) {
            if (auto [Name, Registration] = co_await OnRegistered;
                Registration.GetStructType() == &Struct || Registration.GetSoftStructType() == &Struct) {
                co_return &Registration;
            }
        }
    }
#endif
} // namespace Retro
#endif