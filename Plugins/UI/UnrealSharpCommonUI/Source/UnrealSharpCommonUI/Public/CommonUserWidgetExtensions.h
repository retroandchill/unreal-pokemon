// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "CommonUserWidgetExtensions.generated.h"

class UCommonUserWidget;
struct FCSBindUIActionArgs;

/**
 *
 */
UCLASS(meta = (InternalType))
class UNREALSHARPCOMMONUI_API UCommonUserWidgetExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static TArray<FUIActionBindingHandle> GetActionBindings(const UCommonUserWidget *Widget);

    UFUNCTION(meta = (ScriptMethod))
    static FUIActionBindingHandle RegisterUIActionBinding(UCommonUserWidget *Widget,
                                                          const FCSBindUIActionArgs &BindActionArgs);

    UFUNCTION(meta = (ScriptMethod))
    static void AddActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs);

    UFUNCTION(meta = (ScriptMethod))
    static void RemoveActionBinding(UCommonUserWidget *Widget, FUIActionBindingHandle BindActionArgs);
};
