// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonInputExtensions.generated.h"

class UInputAction;
/**
 *
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UCommonInputExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static bool IsEnhancedInputSupportEnabled();

    UFUNCTION(meta = (ScriptMethod))
    static UInputAction *GetEnhancedInputBackAction();

    UFUNCTION(meta = (ScriptMethod))
    static FDataTableRowHandle GetDefaultBackAction();
};
