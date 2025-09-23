// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "expected.hpp"
#include "RPGEntitySaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "RPGSavingSubsystem.generated.h"

class UComponentSaver;
class UEntitySaver;
/**
 *
 */
UCLASS(DisplayName = "RPG Saving Subsystem")
class RPGSAVING_API URPGSavingSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

  public:
    void Initialize(FSubsystemCollectionBase &Collection) override;

    TOptional<FRPGEntitySaveDataHandle> SaveEntity(const URPGEntity *Entity) const;

    TOptional<FRPGComponentSaveDataHandle> SaveComponent(const URPGComponent *Component) const;

    bool LoadEntity(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    bool LoadComponent(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

  private:
    UEntitySaver *GetEntitySaver(const TSubclassOf<URPGEntity> EntityClass) const;

    UComponentSaver *GetComponentSaver(const TSubclassOf<URPGComponent> ComponentClass) const;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UEntitySaver>> EntitySavers;

    UPROPERTY(Transient)
    mutable TMap<TSubclassOf<URPGEntity>, TObjectPtr<UEntitySaver>> EntitySaverCache;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UComponentSaver>> ComponentSavers;

    UPROPERTY(Transient)
    mutable TMap<TSubclassOf<URPGComponent>, TObjectPtr<UComponentSaver>> ComponentSaverCache;
};
