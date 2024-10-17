// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Variants/VariantObjectUtilities.h"
#include "Blueprint/BlueprintExceptionInfo.h"
#include "Ranges/Variants/VariantObjectStruct.h"

void UVariantObjectUtilities::CreateVariantFromObject(const UObject *Object, uint8 &Variant) {
    // We should never hit this! Stubbed to avoid NoExport on the class.
    check(false)
}

DEFINE_FUNCTION(UVariantObjectUtilities::execCreateVariantFromObject) {
    P_GET_OBJECT(UObject, Object)

    Stack.StepCompiledIn<FStructProperty>(nullptr);
    auto VariantPtr = Stack.MostRecentPropertyAddress;
    P_FINISH

    try {
        const auto StructProp = CastField<FStructProperty>(Stack.MostRecentProperty);
        if (StructProp == nullptr || VariantPtr == nullptr) {
            throw UE::Ranges::FBlueprintException(EBlueprintExceptionType::AccessViolation,
                                                  NSLOCTEXT("CreateVariantFromObject", "MissingOutputProperty",
                                                            "Failed to resolve the output parameter for CreateVariantFromObject."));
        }
        
        
        static auto &Registry = UE::Ranges::FVariantObjectStructRegistry::Get();
        auto StructInfo = Registry.GetVariantStructData(*StructProp->Struct);
        if (!StructInfo.IsSet()) {
            throw UE::Ranges::FBlueprintException(
                EBlueprintExceptionType::AccessViolation,
                NSLOCTEXT(
                    "CreateVariantFromObject", "IncompatibleStructType",
                    "Incompatible struct type; the supplied struct is not a registred variant struct."));
        }

        P_NATIVE_BEGIN
            StructInfo->SetStructValue(Object, *StructProp, VariantPtr);
        P_NATIVE_END

    } catch (const UE::Ranges::FBlueprintException &Exception) {
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, Exception.GetExceptionInfo());
    }
}