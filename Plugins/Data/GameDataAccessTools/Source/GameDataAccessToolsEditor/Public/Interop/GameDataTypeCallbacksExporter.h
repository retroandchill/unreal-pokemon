// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "DataHandle.h"
#include "UObject/Object.h"

#include "GameDataTypeCallbacksExporter.generated.h"

struct FGameDataTypeManagedActions;

/**
 *
 */
UCLASS()
class GAMEDATAACCESSTOOLSEDITOR_API UGameDataTypeCallbacksExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static void SetActions(const FGameDataTypeManagedActions &Actions);

    UNREALSHARP_FUNCTION()
    static void AddActionInformation(TArray<FDataHandleEntry> &DataHandles, FDataHandleEntry *Entry);
};
