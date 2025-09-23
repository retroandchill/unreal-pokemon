// "Unreal Pokémon" created by Retro & Chill.

#include "Savers/EntitySaver.h"
#include "LogRPGSaving.h"
#include "RPGComponent.h"
#include "RPGEntity.h"
#include "RPGSavingSubsystem.h"

bool UEntitySaver::Supports(const TSubclassOf<URPGEntity> Entity) const
{
    if (K2_Supports(Entity))
    {
        return true;
    }

    return NativeSupports(Entity);
}

FRPGEntitySaveDataHandle UEntitySaver::SaveData(const URPGEntity *Entity) const
{
    FRPGEntitySaveDataHandle Data(Entity->GetClass(), GetVersion());

    NativeSaveData(Entity, Data);
    K2_SaveData(Entity, Data);

    return Data;
}

void UEntitySaver::LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const
{
    NativeLoadData(Entity, SaveData);
    K2_LoadData(Entity, SaveData);
}

void UEntitySaver::NativeSaveData(const URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const
{
    const auto *Subsystem = CastChecked<URPGSavingSubsystem>(GetOuter());

    for (TFieldIterator<FProperty> PropIt(Entity->GetClass()); PropIt; ++PropIt)
    {
        auto *ObjProp = CastField<FObjectProperty>(*PropIt);
        if (ObjProp == nullptr || !ObjProp->PropertyClass->IsChildOf(URPGComponent::StaticClass()))
            continue;

        const auto *Component = Cast<URPGComponent>(ObjProp->GetObjectPropertyValue_InContainer(Entity));
        if (Component == nullptr)
            continue;

        if (auto Data = Subsystem->SaveComponent(Component); Data.IsSet())
        {
            SaveData.GetRequiredComponents().Emplace(ObjProp, *Data);
        }
    }

    TMap<UClass *, int32> AdditionalComponentCount;
    for (const URPGComponent *Component : Entity->GetAdditionalComponents())
    {
        auto &Count = AdditionalComponentCount.FindOrAdd(Component->GetClass(), 0);
        if (auto Data = Subsystem->SaveComponent(Component); Data.IsSet())
        {
            SaveData.GetAdditionalComponents().Emplace(FAdditionalComponentDataKey(Component->GetClass(), Count),
                                                       *Data);
            Count++;
        }
    }
}

void UEntitySaver::NativeLoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const
{
    const auto *Subsystem = CastChecked<URPGSavingSubsystem>(GetOuter());

    if (SaveData.GetEntityClass() != Entity->GetClass())
    {
        UE_LOG(LogRPGSaving, Warning, TEXT("Expected entity %s to be of clas %s, but it was %s"), *Entity->GetName(),
               *SaveData.GetEntityClass()->GetName(), *Entity->GetClass()->GetName())
    }

    for (auto &[Property, ComponentData] : SaveData.GetRequiredComponents())
    {
        if (Property == nullptr || !Entity->GetClass()->IsChildOf(Property->PropertyClass))
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load required component %s for entity %s"),
                   *Property.ToString(), *Entity->GetName())
            continue;
        }

        if (auto *Component = Cast<URPGComponent>(Property->GetObjectPropertyValue_InContainer(Entity));
            !Subsystem->LoadComponent(Component, ComponentData))
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load required component %s for entity %s"),
                   *Property.ToString(), *Entity->GetName())
        }
    }

    TMap<UClass *, int32> AdditionalComponentCount;
    TSet<FAdditionalComponentDataKey> LoadedAdditionalComponents;
    for (URPGComponent *Component : Entity->GetAdditionalComponents())
    {
        auto &Count = AdditionalComponentCount.FindOrAdd(Component->GetClass(), 0);
        FAdditionalComponentDataKey Key(Component->GetClass(), Count);
        if (const auto *ComponentData = SaveData.GetAdditionalComponents().Find(Key);
            ComponentData == nullptr || !Subsystem->LoadComponent(Component, *ComponentData))
        {
            UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load additional component %s for entity %s"),
                   *Component->GetClass()->GetName(), *Entity->GetName())
            continue;
        }

        LoadedAdditionalComponents.Emplace(Key);
        Count++;
    }

    for (auto &[Key, ComponentData] : SaveData.GetAdditionalComponents())
    {
        if (LoadedAdditionalComponents.Contains(Key))
            continue;

        UE_LOG(LogRPGSaving, Warning, TEXT("Failed to load additional component %s index %d for entity %s"),
               *Key.ComponentClass->GetName(), Key.Index, *Entity->GetName())
    }
}