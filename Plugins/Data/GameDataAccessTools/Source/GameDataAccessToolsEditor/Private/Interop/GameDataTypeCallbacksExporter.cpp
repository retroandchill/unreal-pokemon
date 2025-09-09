// "Unreal Pokémon" created by Retro & Chill.


#include "Interop/GameDataTypeCallbacksExporter.h"
#include "Interop/GameDataTypeManagedCallbacks.h"

void UGameDataTypeCallbacksExporter::SetActions(const FGameDataTypeManagedActions &Actions)
{
    return FGameDataTypeManagedCallbacks::Get().SetActions(Actions);
}

void UGameDataTypeCallbacksExporter::AddActionInformation(TArray<FDataHandleEntry> &DataHandles, FDataHandleEntry* Entry)
{
    DataHandles.Emplace(MoveTemp(*Entry));
}