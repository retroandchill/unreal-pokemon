
#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Optional/MakeStrong.h"
#include "Ranges/Optional/Map.h"

namespace UE::Ranges {
    FVariantObjectStructRegistry & FVariantObjectStructRegistry::Get() {
        static FVariantObjectStructRegistry Instance;
        return Instance;
    }

    TOptional<UScriptStruct &> FVariantObjectStructRegistry::GetStruct(const UScriptStruct &StructName) {
        return Optionals::OfNullable(RegisteredStructs.Find(StructName.GetFName())) |
            Optionals::MakeStrong;
    }
}