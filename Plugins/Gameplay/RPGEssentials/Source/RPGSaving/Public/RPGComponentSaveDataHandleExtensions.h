// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGEntitySaveData.h"

#include "RPGComponentSaveDataHandleExtensions.generated.h"

class URPGComponent;
struct FRPGComponentSaveDataHandle;
/**
 *
 */
UCLASS(meta = (InternalType))
class RPGSAVING_API URPGComponentSaveDataHandleExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Data))
    static FRPGComponentSaveDataHandle CreateNew(TSoftClassPtr<URPGComponent> ComponentClass, const int32 &Data,
                                                 int32 Version = 1);
    DECLARE_FUNCTION(execCreateNew);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static int32 GetVersion(const FRPGComponentSaveDataHandle &SaveData);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static const TSoftClassPtr<URPGComponent> &GetComponentClass(const FRPGComponentSaveDataHandle &SaveData);

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = OutData, ExtensionMethod))
    static bool GetData(const FRPGComponentSaveDataHandle &SaveData, int32 &OutData);
    DECLARE_FUNCTION(execGetData);

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = Data, ExtensionMethod))
    static void SetData(const FRPGComponentSaveDataHandle &SaveData, const int32 &Data);
    DECLARE_FUNCTION(execSetData);
};
