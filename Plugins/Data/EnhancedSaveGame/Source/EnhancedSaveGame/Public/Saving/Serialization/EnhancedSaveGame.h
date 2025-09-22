// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"

#include "EnhancedSaveGame.generated.h"

/**
 *
 */
UCLASS()
class ENHANCEDSAVEGAME_API UEnhancedSaveGame : public USaveGame
{
    GENERATED_BODY()

  public:
    UFUNCTION(CustomThunk, meta = (ScriptMethod))
    bool TryGetData(FGameplayTag Tag, int32 &OutStructData) const;
    DECLARE_FUNCTION(execTryGetData);

    UFUNCTION(CustomThunk, meta = (ScriptMethod, CustomStructureParam = StructData))
    void AddData(FGameplayTag Tag, const int32 &StructData);
    DECLARE_FUNCTION(execAddData);

    UFUNCTION(meta = (ScriptMethod))
    bool HasData(FGameplayTag Tag) const;

  private:
    UPROPERTY(SaveGame)
    TMap<FGameplayTag, FInstancedStruct> Data;
};
