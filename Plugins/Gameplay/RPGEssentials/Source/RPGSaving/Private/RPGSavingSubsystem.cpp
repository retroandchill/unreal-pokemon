// "Unreal Pokémon" created by Retro & Chill.

#include "RPGSavingSubsystem.h"
#include "RangeV3.h"
#include "RPGComponent.h"
#include "RPGEntity.h"
#include "Savers/ComponentSaver.h"
#include "Savers/EntitySaver.h"

static bool CompareSavers(const USaverBase *A, const USaverBase *B)
{
    return A->GetPriority() < B->GetPriority();
}

void URPGSavingSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    for (const auto *Class : TObjectView<UClass>() | ranges::views::filter([](const UClass *C) {
                                 return !C->HasAnyClassFlags(CLASS_Abstract) && !C->GetName().StartsWith("SKEL_");
                             }))
    {
        if (Class->IsChildOf<UEntitySaver>())
        {
            EntitySavers.Emplace(NewObject<UEntitySaver>(this, Class));
        }
        else if (Class->IsChildOf<UComponentSaver>())
        {
            ComponentSavers.Emplace(NewObject<UComponentSaver>(this, Class));
        }
    }
}

TOptional<FRPGEntitySaveDataHandle> URPGSavingSubsystem::SaveEntity(const URPGEntity *Entity) const
{
    if (const auto *EntitySaver = GetEntitySaver(Entity->GetClass()); EntitySaver != nullptr)
    {
        return EntitySaver->SaveData(Entity);
    }

    return NullOpt;
}

TOptional<FRPGComponentSaveDataHandle> URPGSavingSubsystem::SaveComponent(const URPGComponent *Component) const
{
    if (const auto *ComponentSaver = GetComponentSaver(Component->GetClass()); ComponentSaver != nullptr)
    {
        return ComponentSaver->SaveData(Component);
    }

    return NullOpt;
}

bool URPGSavingSubsystem::LoadEntity(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const
{
    const auto *EntitySaver = GetEntitySaver(Entity->GetClass());
    if (EntitySaver == nullptr)
    {
        return false;
    }

    EntitySaver->LoadData(Entity, SaveData);
    return true;
}

bool URPGSavingSubsystem::LoadComponent(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const
{
    const auto *ComponentSaver = GetComponentSaver(Component->GetClass());
    if (ComponentSaver == nullptr)
    {
        return false;
    }

    ComponentSaver->LoadData(Component, SaveData);
    return true;
}

UEntitySaver *URPGSavingSubsystem::GetEntitySaver(const TSubclassOf<URPGEntity> EntityClass) const
{
    if (const auto *ExistingSaver = EntitySaverCache.Find(EntityClass); ExistingSaver != nullptr)
    {
        return *ExistingSaver;
    }

    auto ValidSavers = EntitySavers | ranges::views::filter([EntityClass](const UEntitySaver *Saver) {
                           return Saver->Supports(EntityClass);
                       });
    if (const auto SpecializedSaver = ranges::min_element(ValidSavers, CompareSavers);
        SpecializedSaver != ValidSavers.end())
    {
        EntitySaverCache.Emplace(EntityClass, *SpecializedSaver);
        return *SpecializedSaver;
    }

    EntitySaverCache.Emplace(EntityClass, nullptr);
    return nullptr;
}

UComponentSaver *URPGSavingSubsystem::GetComponentSaver(const TSubclassOf<URPGComponent> ComponentClass) const
{
    if (const auto *ExistingSaver = ComponentSaverCache.Find(ComponentClass); ExistingSaver != nullptr)
    {
        return *ExistingSaver;
    }

    auto ValidSavers = ComponentSavers | ranges::views::filter([ComponentClass](const UComponentSaver *Saver) {
                           return Saver->Supports(ComponentClass);
                       });
    if (const auto SpecializedSaver = ranges::min_element(ValidSavers, CompareSavers);
        SpecializedSaver != ValidSavers.end())
    {
        ComponentSaverCache.Emplace(ComponentClass, *SpecializedSaver);
        return *SpecializedSaver;
    }

    ComponentSaverCache.Emplace(ComponentClass, nullptr);
    return nullptr;
}