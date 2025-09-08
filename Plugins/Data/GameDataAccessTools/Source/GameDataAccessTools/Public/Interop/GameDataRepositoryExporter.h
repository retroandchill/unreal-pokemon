// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "GameDataRepositoryExporter.generated.h"

class UGameDataRepository;
/**
 *
 */
UCLASS()
class GAMEDATAACCESSTOOLS_API UGameDataRepositoryExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static const FScriptArray *GetEntries(const UGameDataRepository *Repository);

    UNREALSHARP_FUNCTION()
    static const uint8 *GetEntryByID(const UGameDataRepository *Repository, FName ID);

    UNREALSHARP_FUNCTION()
    static const uint8 *GetEntryByIndex(const UGameDataRepository *Repository, int32 Index);
};
