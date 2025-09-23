// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "RPGEntitySaveData.h"
#include "SaverBase.h"
#include "UObject/Object.h"

#include "ComponentSaver.generated.h"

/**
 *
 */
UCLASS()
class RPGSAVING_API UComponentSaver : public USaverBase
{
    GENERATED_BODY()

  public:
    bool Supports(TSubclassOf<URPGComponent> Component) const;

    FRPGComponentSaveDataHandle SaveData(const URPGComponent *Component) const;

    void LoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

  protected:
    virtual bool NativeSupports(TSubclassOf<URPGComponent> Component) const
    {
        return false;
    }

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Supports", Category = "Saving",
              meta = (ScriptName = "Supports"))
    bool K2_Supports(TSubclassOf<URPGComponent> Component) const;

    virtual void NativeSaveData(const URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Save Data", Category = "Saving",
              meta = (ScriptName = "SaveData"))
    void K2_SaveData(const URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    virtual void NativeLoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "Load Data", Category = "Saving",
              meta = (ScriptName = "LoadData"))
    void K2_LoadData(URPGComponent *Component, const FRPGComponentSaveDataHandle &SaveData) const;
};
