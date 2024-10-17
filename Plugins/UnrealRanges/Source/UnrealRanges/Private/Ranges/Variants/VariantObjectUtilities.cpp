// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Blueprint/BlueprintExceptionInfo.h"
#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Optional/Filter.h"
#include "Ranges/Optional/GetPtrOrNull.h"
#include "Ranges/Variants/VariantObjectStruct.h"

void UVariantObjectUtilities::CreateVariantFromObject(const UObject *Object, uint8 &Variant) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
}

DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObject) {
    P_GET_OBJECT(UObject, Object)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
        P_NATIVE_END

    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

EVariantFindResult UVariantObjectUtilities::CreateVariantFromObjectChecked(TSubclassOf<UObject> Class, const UObject *Object, uint8 &Variant) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
    return EVariantFindResult::CastFailed;
}

DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObjectChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OBJECT(UObject, Object)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_GET_RESULT(EVariantFindResult, Result);
        P_NATIVE_BEGIN
            if (auto TypeIndex = StructInfo.GetTypeIndex(Object); Object != nullptr && TypeIndex.IsSet()) {
                Result = EVariantFindResult::CastSucceeded;
                StructInfo.SetStructValue(Object, *StructProp, VariantPtr);
            } else {
                Result = EVariantFindResult::CastFailed;
            }
        P_NATIVE_END

    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

UObject * UVariantObjectUtilities::GetObjectFromVariant(const uint8 &Variant) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
    return nullptr;
}

DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariant) {
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            P_GET_RESULT(UObject*, Object);
        Object = StructInfo.GetValue(*StructProp, VariantPtr).GetPtrOrNull();
        P_NATIVE_END
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}

EVariantFindResult UVariantObjectUtilities::GetObjectFromVariantChecked(TSubclassOf<UObject> Class,
    const uint8 &Variant, UObject *&Object) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
    return EVariantFindResult::CastFailed;
}

DEFINE_FUNCTION(UVariantObjectUtilities::execGetObjectFromVariantChecked) {
    P_GET_OBJECT(UClass, Class)
    P_GET_OPAQUE_STRUCT(StructProp, VariantPtr)
    P_GET_OBJECT_REF(UObject, Object);
    P_FINISH

    try {
        UE::Ranges::ValidateStructProperty(StructProp, VariantPtr);
        auto &StructInfo = UE::Ranges::GetVariantRegistration(*StructProp);
        P_NATIVE_BEGIN
            Object = StructInfo.GetValue(*StructProp, VariantPtr) |
                UE::Optionals::Filter([&Class](const UObject& Obj) { return UE::Ranges::TypesMatch(Obj, *Class); }) |
                UE::Optionals::GetPtrOrNull;
        P_NATIVE_END
        
        P_GET_RESULT(EVariantFindResult, Result);
        Result = Object != nullptr ? EVariantFindResult::CastSucceeded : EVariantFindResult::CastFailed;
    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}