// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "GameDataEntrySerializerExporter.generated.h"

class UAssetGameDataRepository;
/**
 *
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataEntrySerializerExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static void AddToStagingArray(FScriptArray &StagingArray, UAssetGameDataRepository *Repository, const uint8 *Data);
};
