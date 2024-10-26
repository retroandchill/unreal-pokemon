// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Variants/VariantObjectStruct.h"

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::CreateVariantFromObject, const UObject*, uint8&)
DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObject) {
    P_GET_OBJECT(UObject, Object)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
        P_NATIVE_END

    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::CreateVariantFromObjectChecked,
    TSubclassOf<UObject>, const UObject *, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObjectChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OBJECT(UObject, Object)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_GET_RESULT(bool, Result);
        P_NATIVE_BEGIN
            if (auto TypeIndex = StructInfo.GetTypeIndex(Object); Object != nullptr && TypeIndex.IsSet()) {
                Result = true;
                StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
            } else {
                Result = false;
            }
        P_NATIVE_END

    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(UObject *, UVariantObjectUtilities::GetObjectFromVariant, const uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariant) {
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            P_GET_RESULT(UObject*, Object);
            Object = StructInfo.GetValue(*StructProp, VariantPtr).GetPtrOrNull();
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::GetObjectFromVariantChecked,
    TSubclassOf<UObject>, const uint8 &, UObject *&)
DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariantChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_GET_OBJECT_REF(UObject, Object);
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
        // clang-format off
            Object = StructInfo.GetValue(*StructProp, VariantPtr) |
                UE::Optionals::Filter([&Class](const UObject& Obj) { return UE::Ranges::TypesMatch(Obj, *Class); }) |
                UE::Optionals::GetPtrOrNull;
        // clang-format on
        P_NATIVE_END
        
        P_GET_RESULT(bool, Result);
        Result = IsValid(Object);
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::MakeSoftVariantFromVariant, const uint8&, uint8 &)
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromVariant) {
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH
    
    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        UE::Ranges::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            StructInfo.MakeSoftValue(*StructProp, VariantPtr, *SoftStructProp, SoftVariantPtr);
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(void, UVariantObjectUtilities::MakeSoftVariantFromSoftObject, const TSoftObjectPtr<>&, uint8&)
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromSoftObject) {
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
            StructInfo.MakeSoftValue(SoftObject, *SoftStructProp, SoftVariantPtr);
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::MakeSoftVariantFromSoftObjectChecked,
    const TSoftObjectPtr<> &, uint8&);
DEFINE_FUNCTION(UVariantObjectUtilities::execMakeSoftVariantFromSoftObjectChecked) {
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
            P_GET_RESULT(bool, Result);
            if (auto TypeIndex = StructInfo.GetTypeIndex(SoftObject); !SoftObject.IsNull() && TypeIndex.IsSet()) {
                Result = true;
                StructInfo.MakeSoftValue(SoftObject, *SoftStructProp, SoftVariantPtr);
            } else {
                Result = false;
            }
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::SoftVariantCast, UClass *, const uint8 &, TSoftObjectPtr<> &);
DEFINE_FUNCTION(UVariantObjectUtilities::execSoftVariantCast) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_GET_SOFTOBJECT(TSoftObjectPtr<>, SoftObject)
    P_FINISH
    
    try {
        UE::Ranges::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*SoftStructProp);
        P_NATIVE_BEGIN
            P_GET_RESULT(bool, Result);
        if (auto SoftResult = StructInfo.TryGetSoftValue(Class, *SoftStructProp, SoftVariantPtr); SoftResult.IsSet()) {
                Result = true;
                SoftObject = SoftResult->ToSoftObjectPtr();
            } else {
                Result = false;
            }
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

CUSTOM_THUNK_STUB(bool, UVariantObjectUtilities::LoadSynchronous, const uint8 &, uint8&)
DEFINE_FUNCTION(UVariantObjectUtilities::execLoadSynchronous) {
    P_GET_OPAQUE_STRUCT(SoftStructProp, SoftVariantPtr)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)

    try {
        UE::Ranges::ValidateStructProperty(SoftStructProp, SoftVariantPtr);
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        const auto &StructInfo = UE::Ranges::GetVariantRegistration(*SoftStructProp);
        bool bLoaded;
        P_NATIVE_BEGIN
            bLoaded = StructInfo.LoadSynchronous(*SoftStructProp, SoftVariantPtr, *StructProp, VariantPtr);
        P_NATIVE_END
        
        P_GET_RESULT(bool, Result);
        Result = bLoaded;
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}