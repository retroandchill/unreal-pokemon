#include "Ranges/Variants/VariantObjectStruct.h"
#include "Ranges/Optional/Map.h"

namespace UE::Ranges {
    FVariantObjectStructRegistry &FVariantObjectStructRegistry::Get() {
        static FVariantObjectStructRegistry Instance;
        return Instance;
    }

    TOptional<IVariantRegistration &> FVariantObjectStructRegistry::GetVariantStructData(
        const UScriptStruct &Struct) {
        return Optionals::OfNullable(RegisteredStructs.Find(Struct.GetFName())) |
            Optionals::Map(&TUniquePtr<IVariantRegistration>::Get);
    }
}