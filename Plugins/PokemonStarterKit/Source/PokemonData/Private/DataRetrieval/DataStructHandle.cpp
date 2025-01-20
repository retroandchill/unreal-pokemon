// "Unreal Pokémon" created by Retro & Chill.

#include "DataRetrieval/DataStructHandle.h"
#include "Misc/OutputDeviceNull.h"
#include "RetroLib/Blueprints/BlueprintRuntimeUtils.h"
#include "RetroLib/Ranges/Algorithm/To.h"
#include "RetroLib/Ranges/Views/NameAliases.h"
#include <bit>

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
    check(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"))

    auto Options = RowProperty->GetMetaData("GetOptions");
    auto GetOptionsFunction = FindObject<UFunction>(nullptr, *Options, true);
    check(GetOptionsFunction != nullptr && GetOptionsFunction->HasAnyFunctionFlags(EFunctionFlags::FUNC_Static))

    FGetOptionsParams Params;
    Retro::InvokeFunctionIsolated(GetOptionsFunction, Params);

    // clang-format off
    return Params.ReturnValue |
           Retro::Ranges::Views::Transform([](FName N) -> TSharedPtr<FString> {
               return MakeShared<FString>(N.ToString());
           }) |
           Retro::Ranges::To<TArray>();
    // clang-format on
}
#endif

FName Pokemon::Data::FStructWrapper::GetRowID() const {
    check(Struct.get() != nullptr)
    auto StructClass = GetStruct();
    check(StructClass != nullptr)

    auto RowProperty = StructClass->FindPropertyByName(DataStructRowID);
    check(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"))

    auto PropertyContainer = RowProperty->ContainerPtrToValuePtr<void>(Struct.get());
    return TPropertyTypeFundamentals<FName>::GetPropertyValue(PropertyContainer);
}

void Pokemon::Data::FStructWrapper::SetRowID(FName RowID) {
    check(Struct.get() != nullptr)
    auto StructClass = GetStruct();
    check(StructClass != nullptr)

    auto RowProperty = StructClass->FindPropertyByName(DataStructRowID);
    check(RowProperty != nullptr && RowProperty->GetCPPType() == TEXT("FName"))

    auto PropertyContainer = RowProperty->ContainerPtrToValuePtr<void>(Struct.get());
    TPropertyTypeFundamentals<FName>::SetPropertyValue(PropertyContainer, RowID);
}

FString Pokemon::Data::FStructWrapper::ExportText() const {
    FString Ret;
    GetStruct()->ExportText(Ret, Struct.get(), Struct.get(), nullptr, PPF_None, nullptr);
    return Ret;
}

void Pokemon::Data::FStructWrapper::FromExportString(FStringView ExportString, int32 PortFlags) {
    if (ExportString.IsEmpty()) {
        return;
    }

    auto StructClass = GetStruct();
    Struct = std::unique_ptr<void, FStructDestructor>(FMemory::Malloc(StructClass->GetStructureSize()),
                                                      FStructDestructor(StructClass));
    StructClass->InitializeStruct(Struct.get());

    FOutputDeviceNull NullOut;
    StructClass->ImportText(ExportString.GetData(), Struct.get(), nullptr, PortFlags, &NullOut,
                            StructClass->GetFName().ToString(), true);
}

bool UDataStructHandleUtilities::NotEqual_HandleHandle(const FDataStructHandle &, FName) {
    check(false)
    return false;
}

DEFINE_FUNCTION(UDataStructHandleUtilities::execNotEqual_HandleHandle) {
    P_GET_STRUCT_REF(FDataStructHandle, DataHandle)
    P_GET_PROPERTY(FNameProperty, Other)
    P_FINISH

    bool bResult;
    P_NATIVE_BEGIN
        bResult = DataHandle.RowID != Other;
    P_NATIVE_END

    *static_cast<bool *>(RESULT_PARAM) = bResult;
}