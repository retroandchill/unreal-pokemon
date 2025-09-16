// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonActivatableWidgetExtensions.generated.h"

struct FCSBindUIActionArgs;
class UCommonActivatableWidget;

/**
 * 
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UCommonActivatableWidgetExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(meta = (ScriptMethod))
    static TArray<FUIActionBindingHandle> GetActionBindings(const UCommonActivatableWidget* Widget);

    UFUNCTION(meta = (ScriptMethod))
    static FUIActionBindingHandle RegisterUIActionBinding(UCommonActivatableWidget* Widget, const FCSBindUIActionArgs &BindActionArgs);

    UFUNCTION(meta = (ScriptMethod))
    static void AddActionBinding(UCommonActivatableWidget* Widget, FUIActionBindingHandle BindActionArgs);

    UFUNCTION(meta = (ScriptMethod))
    static void RemoveActionBinding(UCommonActivatableWidget* Widget, FUIActionBindingHandle BindActionArgs);

};
