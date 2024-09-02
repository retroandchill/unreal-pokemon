// "Unreal Pokémon" created by Retro & Chill.


#include "DataRetrieval/DataStructHandle.h"
#include "Ranges/Algorithm/ToArray.h"
#include "Ranges/Views/Map.h"

#if WITH_METADATA
bool Pokemon::Data::IsValidDataTableStruct(const UScriptStruct *Struct) {
    if (Struct == nullptr) {
        return false;
    }

    auto RowProperty = Struct->FindPropertyByName(DataStructRowID);
    if (RowProperty == nullptr || RowProperty->GetCPPType() != TEXT("FName")) {
        return false;
    }

    auto Options = RowProperty->GetMetaData("GetOptions");
    if (auto GetOptionsFunction = FindObject<UFunction>(nullptr, *Options, true);
        GetOptionsFunction == nullptr || !GetOptionsFunction->HasAnyFunctionFlags(FUNC_Static)) {
        return false;
    }

    return true;
}

struct FGetOptionsParams {
    TArray<FName> ReturnValue;
};

TArray<TSharedPtr<FString>> Pokemon::Data::FStructWrapper::GetStructOptions() const {
    check(Struct.get() != nullptr)
    auto StructClass = GetStruct();
    check(StructClass != nullptr)

    auto RowProperty = StructClass->FindPropertyByName(DataStructRowID);
    checkf(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"),
           TEXT("Missing FName property: %s"), RowProperty)

    auto Options = RowProperty->GetMetaData("GetOptions");
    auto GetOptionsFunction = FindObject<UFunction>(nullptr, *Options, true);
    checkf(GetOptionsFunction != nullptr && GetOptionsFunction->HasAnyFunctionFlags(EFunctionFlags::FUNC_Static),
           TEXT("Invalid GetOptions: %s"), *Options)

    FGetOptionsParams Params;
    auto GetOptionsCDO = GetOptionsFunction->GetOuterUClass()->GetDefaultObject();
    GetOptionsCDO->ProcessEvent(GetOptionsFunction, &Params);

    // clang-format off
    return Params.ReturnValue |
           UE::Ranges::Map([](FName N) -> TSharedPtr<FString> { return MakeShared<FString>(N.ToString()); }) |
           UE::Ranges::ToArray;
    // clang-format on
}
#endif

FName Pokemon::Data::FStructWrapper::GetRowID() const {
    check(Struct.get() != nullptr)
    auto StructClass = GetStruct();
    check(StructClass != nullptr)

    auto RowProperty = StructClass->FindPropertyByName(DataStructRowID);
    checkf(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"),
           TEXT("Missing FName property: %s"), RowProperty)
    
    auto PropertyContainer = RowProperty->ContainerPtrToValuePtr<void>(Struct.get());
    return TPropertyTypeFundamentals<FName>::GetPropertyValue(PropertyContainer);
}

void Pokemon::Data::FStructWrapper::SetRowID(FName RowID) {
    check(Struct.get() != nullptr)
    auto StructClass = GetStruct();
    check(StructClass != nullptr)

    auto RowProperty = StructClass->FindPropertyByName(DataStructRowID);
    checkf(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"),
           TEXT("Missing FName property: %s"), RowProperty)

    auto PropertyContainer = RowProperty->ContainerPtrToValuePtr<void>(Struct.get());
    TPropertyTypeFundamentals<FName>::SetPropertyValue(PropertyContainer, RowID);
}

FString Pokemon::Data::FStructWrapper::ExportText() const {
    FString Ret;
    GetStruct()->ExportText(Ret, Struct.get(), Struct.get(), nullptr, PPF_None, nullptr);
    return Ret;
}