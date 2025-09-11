// "Unreal Pokémon" created by Retro & Chill.

#include "Interop/GameDataTypeManagedCallbacks.h"

FGameDataTypeManagedCallbacks &FGameDataTypeManagedCallbacks::Get()
{
    static FGameDataTypeManagedCallbacks Instance;
    return Instance;
}

TArray<FDataHandleEntry> FGameDataTypeManagedCallbacks::GetDataHandleOptions(const FGCHandle &Handle) const
{
    TArray<FDataHandleEntry> DataHandles;
    Actions.GetDataHandleOptions(Handle.GetHandle(), &DataHandles);
    return DataHandles;
}