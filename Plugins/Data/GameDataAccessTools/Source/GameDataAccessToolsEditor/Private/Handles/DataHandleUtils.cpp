// "Unreal Pokémon" created by Retro & Chill.

#include "Handles/DataHandleUtils.h"
#include "Interop/GameDataTypeManagedCallbacks.h"
#include "TypeGenerator/CSScriptStruct.h"

bool GameData::IsDataHandleStruct(UScriptStruct *Struct)
{
    const auto ManagedStruct = Cast<UCSScriptStruct>(Struct);
    if (ManagedStruct == nullptr)
    {
        return false;
    }

    if (CastField<FNameProperty>(ManagedStruct->FindPropertyByName("ID")) == nullptr)
    {
        return false;
    }

    const auto ManagedType = ManagedStruct->GetManagedTypeInfo()->GetManagedTypeHandle();
    return FGameDataTypeManagedCallbacks::Get().IsValidGameDataHandle(*ManagedType);
}