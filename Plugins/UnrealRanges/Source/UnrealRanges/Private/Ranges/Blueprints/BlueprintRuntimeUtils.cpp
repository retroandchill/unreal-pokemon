// "Unreal Pokémon" created by Retro & Chill.


#include "Ranges/Blueprints/BlueprintRuntimeUtils.h"
#include "Ranges/Variants/VariantObjectStruct.h"


UE::Ranges::IVariantRegistration & UE::Ranges::GetVariantRegistration(const FStructProperty &Property) {
    static auto &Registry = FVariantObjectStructRegistry::Get();
    auto StructInfo = Registry.GetVariantStructData(*Property.Struct);
    if (!StructInfo.IsSet()) {
        throw FVariantException(
            EBlueprintExceptionType::AccessViolation,
            NSLOCTEXT(
                "GetVariantRegistration", "IncompatibleStructType",
                "Incompatible struct type; the supplied struct is not a registered variant struct."));
    }

    return *StructInfo;
}