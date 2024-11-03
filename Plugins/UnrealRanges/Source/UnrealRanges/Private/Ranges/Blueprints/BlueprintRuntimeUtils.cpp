// "Unreal Pokémon" created by Retro & Chill.

#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Variants/VariantObjectStruct.h"

void UE::Ranges::ValidateStructsMatch(const FStructProperty &Property, const UScriptStruct *Struct) {
    if (Property.Struct != Struct) {
        throw FTypeException(EBlueprintExceptionType::AccessViolation,
                             NSLOCTEXT("ValidateStructsMatch", "IncompatibleProperty",
                                       "Incompatible output parameter; the supplied struct does not have the "
                                       "same layout as what is expected for a variant object struct."));
    }
}

void UE::Ranges::ValidateEnumsMatch(const FEnumProperty &Property, const UEnum *Enum) {
    if (Property.GetEnum() != Enum) {
        throw FTypeException(EBlueprintExceptionType::AccessViolation,
                             NSLOCTEXT("ValidateEnumsMatch", "IncompatibleProperty",
                                       "Incompatible output parameter; the supplied enum is not the same type as the"
                                       "expected type."));
    }
}

UE::Ranges::IVariantRegistration &UE::Ranges::GetVariantRegistration(const FStructProperty &Property) {
    static auto &Registry = FVariantObjectStructRegistry::Get();
    auto StructInfo = Registry.GetVariantStructData(*Property.Struct);
    if (!StructInfo.IsSet()) {
        throw FVariantException(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT("GetVariantRegistration", "IncompatibleStructType",
                      "Incompatible struct type; the supplied struct is not a registered variant struct."));
    }

    return *StructInfo;
}