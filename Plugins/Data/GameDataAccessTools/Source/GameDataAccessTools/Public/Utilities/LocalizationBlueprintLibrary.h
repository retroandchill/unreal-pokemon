// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LocalizationBlueprintLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDATAACCESSTOOLS_API ULocalizationBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(meta = (ScriptMethod, ExtensionMethod))
    static FString ToLocalizedString(const FText& Text);

    UFUNCTION(meta=(ScriptMethod, ExtensionMethod))
    static bool TryGetNamespace(const FText& Text, FString& OutNamespace);

    UFUNCTION(meta=(ScriptMethod, ExtensionMethod))
    static bool TryGetKey(const FText& Text, FString& OutKey);

};
