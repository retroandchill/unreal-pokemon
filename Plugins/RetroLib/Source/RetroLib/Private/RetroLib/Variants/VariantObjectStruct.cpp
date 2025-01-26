// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Variants/VariantObjectStruct.h"
#include "RetroLib/Optionals/Transform.h"
#include "RetroLib/Optionals/AndThen.h"

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
} // namespace Retro
#endif