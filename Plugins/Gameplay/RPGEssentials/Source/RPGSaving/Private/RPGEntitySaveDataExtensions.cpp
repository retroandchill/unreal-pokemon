// "Unreal Pokémon" created by Retro & Chill.

#include "RPGEntitySaveDataExtensions.h"
#include "OptionalPtr.h"
#include "RPGComponent.h"
#include "RPGEntity.h"
#include "RPGEntitySaveData.h"

int32 URPGEntitySaveDataExtensions::GetVersion(const FRPGEntitySaveDataHandle &SaveData)
{
    return SaveData.GetVersion();
}

TSubclassOf<URPGEntity> URPGEntitySaveDataExtensions::GetEntityClass(const FRPGEntitySaveDataHandle &SaveData)
{
    return SaveData.GetEntityClass();
}

bool URPGEntitySaveDataExtensions::GetRequiredComponent(const FRPGEntitySaveDataHandle &SaveData, FName PropertyName,
                                                        FRPGComponentSaveDataHandle &OutComponentData)
{
    const auto *Handle =
        TOptionalPtr(SaveData.GetEntityClass().Get())
            .Map([PropertyName](const UClass *EntityClass) {
                return CastField<FObjectProperty>(EntityClass->FindPropertyByName(PropertyName));
            })
            .Map([&SaveData](FObjectProperty *Property) { return SaveData.GetRequiredComponents().Find(Property); })
            .Get();

    if (Handle != nullptr)
    {
        OutComponentData = *Handle;
        return true;
    }

    return false;
}

bool URPGEntitySaveDataExtensions::GetAdditionalComponent(const FRPGEntitySaveDataHandle &SaveData,
                                                          const FAdditionalComponentDataKey &Key,
                                                          FRPGComponentSaveDataHandle &OutComponentData)
{
    if (const auto *Component = SaveData.GetAdditionalComponents().Find(Key); Component != nullptr)
    {
        OutComponentData = *Component;
        return true;
    }

    return false;
}