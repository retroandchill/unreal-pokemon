// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "SaverBase.h"
#include "UObject/Object.h"

#include "EntitySaver.generated.h"

class URPGEntity;
/**
 *
 */
UCLASS(Abstract)
class RPGSAVING_API UEntitySaver : public USaverBase
{
    GENERATED_BODY()

  public:
    bool Supports(TSubclassOf<URPGEntity> Entity) const;

    FRPGEntitySaveDataHandle SaveData(const URPGEntity *Entity) const;

    void LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

  protected:
    virtual bool NativeSupports(TSubclassOf<URPGEntity> Entity) const
    {
        return false;
    }

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Supports", Category = "Saving",
              meta = (ScriptName = "Supports"))
    bool K2_Supports(TSubclassOf<URPGEntity> Entity) const;

    virtual void NativeSaveData(const URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Save Data", Category = "Saving",
              meta = (ScriptName = "SaveData"))
    void K2_SaveData(const URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    virtual void NativeLoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Data", Category = "Saving",
              meta = (ScriptName = "LoadData"))
    void K2_LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData) const;
};
