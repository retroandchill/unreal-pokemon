// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGEntitySaveData.h"

#include "RPGSavingExtensions.generated.h"

/**
 *
 */
UCLASS()
class RPGSAVING_API URPGSavingExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(BlueprintCallable, Category = "Saving", meta = (ExtensionMethod))
    static FRPGEntitySaveDataHandle SaveData(const URPGEntity *Entity);

    UFUNCTION(BlueprintCallable, Category = "Saving", meta = (ExtensionMethod))
    static void LoadData(URPGEntity *Entity, const FRPGEntitySaveDataHandle &SaveData);
};
