// "Unreal Pokémon" created by Retro & Chill.

#include "RPGComponentSaveDataHandleExtensions.h"
#include "LogRPGSaving.h"
#include "RPGComponent.h"
#include "RPGEntitySaveData.h"
#include "Unreachable.h"

FRPGComponentSaveDataHandle URPGComponentSaveDataHandleExtensions::CreateNew(
    const TSubclassOf<URPGComponent> ComponentClass, const int32 Version)
{
    return FRPGComponentSaveDataHandle(ComponentClass, Version);
}

int32 URPGComponentSaveDataHandleExtensions::GetVersion(const FRPGComponentSaveDataHandle &SaveData)
{
    return SaveData.GetVersion();
}

TSubclassOf<URPGComponent> URPGComponentSaveDataHandleExtensions::GetComponentClass(
    const FRPGComponentSaveDataHandle &SaveData)
{
    return SaveData.GetComponentClass();
}

bool URPGComponentSaveDataHandleExtensions::GetData(const FRPGComponentSaveDataHandle &, int32 &)
{
    checkf(false, TEXT("This should not be called"));
    UE::Unreachable();
}

DEFINE_FUNCTION(URPGComponentSaveDataHandleExtensions::execGetData)
{
    P_GET_STRUCT(FRPGComponentSaveDataHandle, SaveData);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto ReturnValueProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    auto *ReturnValueStruct = Stack.MostRecentPropertyAddress;

    P_FINISH;

    P_NATIVE_BEGIN

    if (!SaveData.GetData().IsValid())
    {
        UE_LOG(LogRPGSaving, Error, TEXT("Struct stored in save data was invalid!"))
        *static_cast<bool *>(RESULT_PARAM) = false;
        return;
    }

    if (!SaveData.GetData().GetScriptStruct() != ReturnValueProperty->Struct)
    {
        UE_LOG(LogRPGSaving, Error, TEXT("Expected struct %s, but got %s"), *ReturnValueProperty->Struct->GetName(),
               *SaveData.GetData().GetScriptStruct()->GetName())
        *static_cast<bool *>(RESULT_PARAM) = false;
        return;
    }

    ReturnValueProperty->CopyValuesInternal(ReturnValueStruct, SaveData.GetData().GetMemory(), 1);
    *static_cast<bool *>(RESULT_PARAM) = true;

    P_NATIVE_END
}

void URPGComponentSaveDataHandleExtensions::SetData(const FRPGComponentSaveDataHandle &, const int32 &)
{
    checkf(false, TEXT("This should not be called"));
    UE::Unreachable();
}

DEFINE_FUNCTION(URPGComponentSaveDataHandleExtensions::execSetData)
{
    P_GET_STRUCT(FRPGComponentSaveDataHandle, SaveData);
    Stack.StepCompiledIn<FStructProperty>(nullptr);
    const auto DataProperty = CastFieldChecked<FStructProperty>(Stack.MostRecentProperty);
    const auto *DataStruct = Stack.MostRecentPropertyAddress;

    P_FINISH;

    P_NATIVE_BEGIN

    SaveData.GetData().InitializeAs(DataProperty->Struct, DataStruct);

    P_NATIVE_END
}

const UScriptStruct *URPGComponentSaveDataHandleExtensions::GetSaveDataStruct(
    const FRPGComponentSaveDataHandle &SaveData)
{
    return SaveData.GetData().GetScriptStruct();
}