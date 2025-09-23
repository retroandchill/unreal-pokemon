// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "RPGEntitySaveDataExtensions.generated.h"

struct FAdditionalComponentDataKey;
class URPGComponent;
struct FRPGComponentSaveDataHandle;
class URPGEntity;
struct FRPGEntitySaveDataHandle;
/**
 *
 */
UCLASS(meta = (InternalType))
class RPGSAVING_API URPGEntitySaveDataExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static int32 GetVersion(const FRPGEntitySaveDataHandle &SaveData);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static TSubclassOf<URPGEntity> GetEntityClass(const FRPGEntitySaveDataHandle &SaveData);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetRequiredComponent(const FRPGEntitySaveDataHandle &SaveData, FName PropertyName,
                                     FRPGComponentSaveDataHandle &OutComponentData);

    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static bool GetAdditionalComponent(const FRPGEntitySaveDataHandle &SaveData, const FAdditionalComponentDataKey &Key,
                                       FRPGComponentSaveDataHandle &OutComponentData);
};
