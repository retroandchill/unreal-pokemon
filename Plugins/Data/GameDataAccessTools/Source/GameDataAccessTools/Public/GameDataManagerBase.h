// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "GameDataManagerBase.generated.h"

class UAssetGameDataRepository;
/**
 *
 */
UCLASS(Abstract)
class GAMEDATAACCESSTOOLS_API UGameDataManagerBase : public UObject
{
    GENERATED_BODY()

  protected:
    UFUNCTION(meta = (ScriptMethod, DeterminesOutputType = "ManagerClass", DynamicOutputParam = "ReturnValue"))
    static UGameDataManagerBase *Create(TSubclassOf<UGameDataManagerBase> ManagerClass, FName Name);

    UFUNCTION(meta = (ScriptMethod))
    void Destroy();

    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataManager")
    void Initialize();

    UFUNCTION(BlueprintImplementableEvent, Category = "GameDataManager")
    void Deinitialize();
};
