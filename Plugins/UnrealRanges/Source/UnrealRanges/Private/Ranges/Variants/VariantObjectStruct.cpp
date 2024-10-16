#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Optional/Map.h"

namespace UE::Ranges {
    FRegisteredVariantStruct::FRegisteredVariantStruct(UScriptStruct *StructType, UScriptStruct *SoftStructType,
                                                       TOptional<uint64> (*IndexFunction) (const UObject*),
                                                       TArray<UClass *> &&AllowedClasses): StructType(StructType),
        SoftStructType(SoftStructType),
        TypeIndexFunction(MoveTemp(IndexFunction)),
        AllowedClasses(MoveTemp(AllowedClasses)) {
    }

    FVariantObjectStructRegistry &FVariantObjectStructRegistry::Get() {
        static FVariantObjectStructRegistry Instance;
        return Instance;
    }

    TOptional<FRegisteredVariantStruct &> FVariantObjectStructRegistry::GetVariantStructData(
        const UScriptStruct &Struct) {
        return Optionals::OfNullable(RegisteredStructs.Find(Struct.GetFName()));
    }
}