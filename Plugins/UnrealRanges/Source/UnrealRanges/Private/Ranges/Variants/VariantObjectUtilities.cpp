// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Blueprint/BlueprintExceptionInfo.h"
#include "Ranges/Variants/VariantObjectStruct.h"

void UVariantObjectUtilities::CreateVariantFromObject(const UScriptStruct* DataStruct, const UObject *Object, int32 &Variant) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
}

DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObject) {
    P_GET_OBJECT(UScriptStruct, DataStruct)
    P_GET_OBJECT(UObject, Object)

    Stack.StepCompiledIn<FStructProperty>(nullptr);
    void *VariantPtr = Stack.MostRecentPropertyAddress;
    P_FINISH

    static auto& Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
    auto StructInfo = Registry.GetVariantStructData(*DataStruct);
    if (!StructInfo.IsSet()) {
        FBlueprintExceptionInfo ExceptionInfo(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT(
                    "CreateVariantFromObject", "IncompatibleStructType",
                    "Incompatible struct type; the supplied struct is not a registred variant struct."));
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        return;
    }

    const auto StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
    if (StructProp == nullptr || VariantPtr == nullptr) {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("CreateVariantFromObject", "MissingOutputProperty",
                      "Failed to resolve the output parameter for CreateVariantFromObject."));
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        return;
    }

    if (auto OutputType = StructProp->Struct; !FStructUtils::TheSameLayout(OutputType, FVariantObjectTemplate::StaticStruct())) {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT(
                "CreateVariantFromObject", "IncompatibleProperty",
                "Incompatible output parameter; the supplied struct does not have the same layout as what is expected"
                "for a variant object struct."));
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        return;
    }

    auto FoundTypeIndex = StructInfo->GetTypeIndex(Object);
    if (!FoundTypeIndex.IsSet()) {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT(
                "CreateVariantFromObject", "InvalidObjectType",
                "Incompatible object parameter; the supplied object is not of a valid type for this variant object"));
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
        return;
    }

    P_NATIVE_BEGIN
    auto &[ContainedObject, TypeIndex] = *static_cast<FVariantObjectTemplate*>(VariantPtr);
    ContainedObject = Object;
    TypeIndex = *FoundTypeIndex;
    P_NATIVE_END
}