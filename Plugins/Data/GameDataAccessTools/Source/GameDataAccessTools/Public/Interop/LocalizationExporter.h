// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "CSBindsManager.h"
#include "UObject/Object.h"

#include "LocalizationExporter.generated.h"

/**
 *
 */
UCLASS(meta = (InternalType, SkipGlueGeneration))
class GAMEDATAACCESSTOOLS_API ULocalizationExporter : public UObject
{
    GENERATED_BODY()

  public:
    UNREALSHARP_FUNCTION()
    static void CreateLocalizedText(const TCHAR *Namespace, const TCHAR *Key, const TCHAR *DefaultValue,
                                    FText &OutText);
};
