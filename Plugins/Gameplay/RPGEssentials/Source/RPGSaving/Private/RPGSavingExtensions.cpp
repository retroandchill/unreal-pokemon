// "Unreal Pokémon" created by Retro & Chill.

#include "RPGSavingExtensions.h"
#include "LogRPGSaving.h"
#include "RPGComponent.h"
#include "RPGEntity.h"
#include "SaveableComponent.h"
#include "SaveableEntity.h"

FRPGEntitySaveDataHandle URPGSavingExtensions::SaveData(const URPGEntity *Entity)
{
    FRPGEntitySaveDataHandle SaveData(
        Entity->GetClass(), Entity->Implements<USaveableEntity>() ? ISaveableEntity::Execute_GetVersion(Entity) : 1);

    for (TFieldIterator<FProperty> PropIt(Entity->GetClass()); PropIt; ++PropIt)
    {
        auto *ObjProp = CastField<FObjectProperty>(*PropIt);
        if (ObjProp == nullptr || !ObjProp->PropertyClass->IsChildOf(URPGComponent::StaticClass()))
            continue;

        if (const auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(Entity));
            Component != nullptr && Component->Implements<USaveableComponent>())
        {
            SaveData.GetRequiredComponents().Emplace(ObjProp, ISaveableComponent::Execute_CreateSaveData(Component));
        }
    }

    TMap<UClass *, int32> AdditionalComponentCount;
    for (const URPGComponent *Component :
         Entity->GetAdditionalComponents() |
             ranges::views::filter([](const URPGComponent *C) { return C->Implements<USaveableComponent>(); }))
    {
        auto &Count = AdditionalComponentCount.FindOrAdd(Component->GetClass(), 0);
        SaveData.GetAdditionalComponents().Emplace(FAdditionalComponentDataKey(Component->GetClass(), Count),
                                                   ISaveableComponent::Execute_CreateSaveData(Component));
        Count++;
    }

    if (Entity->Implements<USaveableEntity>())
    {
        ISaveableEntity::Execute_SaveCustomData(Entity, SaveData);
    }

    return SaveData;
}

void URPGSavingExtensions::LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData)
{
    if (SaveData.GetEntityClass().LoadSynchronous() != Entity->GetClass())
    {
        UE_LOG(LogRPGSaving, Warning, TEXT("Expected entity %s to be of clas %s, but it was %s"), *Entity->GetName(),
               *SaveData.GetEntityClass().GetAssetName(), *Entity->GetClass()->GetName())
    }

    for (auto &[Property, ComponentData] : SaveData.GetRequiredComponents())
    {
        if (Property == nullptr || !Entity->GetClass()->IsChildOf(Property->PropertyClass))
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load required component %s for entity %s"),
                   *Property.ToString(), *Entity->GetName())
            continue;
        }

        auto *Component = Cast<URPGComponent>(Property->GetObjectPropertyValue_InContainer(Entity));
        if (!Component->Implements<USaveableComponent>())
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load required component %s for entity %s"),
                   *Property.ToString(), *Entity->GetName())
            continue;
        }

        ISaveableComponent::Execute_LoadSaveData(Component, ComponentData);
    }

    TMap<UClass *, int32> AdditionalComponentCount;
    TSet<FAdditionalComponentDataKey> LoadedAdditionalComponents;
    for (URPGComponent *Component :
         Entity->GetAdditionalComponents() |
             ranges::views::filter([](const URPGComponent *C) { return C->Implements<USaveableComponent>(); }))
    {
        auto &Count = AdditionalComponentCount.FindOrAdd(Component->GetClass(), 0);
        FAdditionalComponentDataKey Key(Component->GetClass(), Count);
        const auto *ComponentData = SaveData.GetAdditionalComponents().Find(Key);
        if (ComponentData == nullptr)
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load additional component %s for entity %s"),
                   *Component->GetClass()->GetName(), *Entity->GetName())
            continue;
        }

        ISaveableComponent::Execute_LoadSaveData(Component, *ComponentData);
        LoadedAdditionalComponents.Emplace(MoveTemp(Key));
        Count++;
    }

    for (auto &[Key, ComponentData] : SaveData.GetAdditionalComponents())
    {
        if (LoadedAdditionalComponents.Contains(Key))
            continue;

        UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load additional component %s index %d for entity %s"),
               *Key.ComponentClass.ToString(), Key.Index, *Entity->GetName())
    }

    if (Entity->Implements<USaveableEntity>())
    {
        ISaveableEntity::Execute_LoadCustomData(Entity, SaveData);
    }
}