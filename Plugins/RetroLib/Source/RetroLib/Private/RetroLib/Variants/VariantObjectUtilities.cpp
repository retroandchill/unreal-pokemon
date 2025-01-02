// Fill out your copyright notice in the Description page of Project Settings.

#ifdef __UNREAL__
#include "RetroLib/Variants/VariantObjectUtilities.h"

#include <RetroLib/Optionals/Filter.h>
#include <RetroLib/Optionals/PtrOrNull.h>

#include "RetroLib/Utils/Unreachable.h"
#include "RetroLib/RetroLibMacros.h"
#include "RetroLib/Blueprints/BlueprintRuntimeUtils.h"
#include "RetroLib/Blueprints/Properties.h"
#include "RetroLib/Exceptions/ConvertException.h"
#include "RetroLib/Variants/VariantObjectStruct.h"

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::CreateVariantFromObject, const UObject *, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObject) {
    P_GET_WILDCARD_PARAM(ObjectParam, ObjectData)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*StructProp);
        auto Object = Retro::GetObjectFromProperty(ObjectParam, ObjectData);
        P_NATIVE_BEGIN
        StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
        P_NATIVE_END

    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::CreateVariantFromObjectChecked, TSubclassOf<UObject>, const UObject *,
                  uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObjectChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OBJECT(UObject, Object)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*StructProp);
        P_GET_RESULT(bool, Result);
        P_NATIVE_BEGIN
        if (auto TypeIndex = StructInfo.GetTypeIndex(Object); Object != nullptr && TypeIndex.IsSet()) {
            Result = true;
            StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
        } else {
            Result = false;
        }
        P_NATIVE_END

    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(UObject *, UVariantObjectUtilities::GetObjectFromVariant, const uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariant) {
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
        P_GET_RESULT(UObject *, Object);
        Object = StructInfo.GetValue(*StructProp, VariantPtr).GetPtrOrNull();
        P_NATIVE_END
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::GetObjectFromVariantChecked, TSubclassOf<UObject>, const uint8 &,
                  UObject *&)
DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariantChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_GET_WILDCARD_PARAM(ObjectProp, ObjectData)
    P_FINISH

    try {
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*StructProp);
        UObject *Object;
        P_NATIVE_BEGIN
        // clang-format off
            Object = StructInfo.GetValue(*StructProp, VariantPtr) |
                Retro::Optionals::Filter([&Class](const UObject& Obj) { return Retro::TypesMatch(Obj, *Class); }) |
                Retro::Optionals::PtrOrNull;
        // clang-format on
        Retro::AssignObjectToProperty(ObjectProp, ObjectData, Object);
        P_NATIVE_END

        P_GET_RESULT(bool, Result);
        Result = IsValid(Object);
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::MakeSoftVariantFromVariant, const uint8 &, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromVariant) {
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        Retro::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
        StructInfo.MakeSoftValue(*StructProp, VariantPtr, *SoftStructProp, SoftVariantPtr);
        P_NATIVE_END
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::MakeSoftVariantFromSoftObject, const TSoftObjectPtr<> &, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromSoftObject) {
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
        StructInfo.MakeSoftValue(SoftObject, *SoftStructProp, SoftVariantPtr);
        P_NATIVE_END
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::MakeSoftVariantFromSoftObjectChecked, const TSoftObjectPtr<> &,
                  uint8 &);
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromSoftObjectChecked) {
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
        P_GET_RESULT(bool, Result);
        if (auto TypeIndex = StructInfo.GetTypeIndex(SoftObject); !SoftObject.IsNull() && TypeIndex.IsSet()) {
            Result = true;
            StructInfo.MakeSoftValue(SoftObject, *SoftStructProp, SoftVariantPtr);
        } else {
            Result = false;
        }
        P_NATIVE_END
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::SoftVariantCast, UClass *, const uint8 &, TSoftObjectPtr<> &);
DEFINE_FUNCTION(UVariantObjectUtilities::execSoftVariantCast) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_FINISH

    try {
        Retro::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
        P_GET_RESULT(bool, Result);
        if (auto SoftResult = StructInfo.TryGetSoftValue(Class, *SoftStructProp, SoftVariantPtr); SoftResult.IsSet()) {
            Result = true;
            SoftObject = SoftResult->ToSoftObjectPtr();
        } else {
            Result = false;
        }
        P_NATIVE_END
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::LoadSynchronous, const uint8 &, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execLoadSynchronous) {
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        Retro::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        Retro::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = Retro::GetVariantRegistration(*SoftStructProp);
        bool bLoaded;
        P_NATIVE_BEGIN
        bLoaded = StructInfo.LoadSynchronous(*SoftStructProp, SoftVariantPtr, *StructProp, VariantPtr);
        P_NATIVE_END

        P_GET_RESULT(bool, Result);
        Result = bLoaded;
    } catch (const Retro::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ConvertException(Exception));
    }
}
#endif