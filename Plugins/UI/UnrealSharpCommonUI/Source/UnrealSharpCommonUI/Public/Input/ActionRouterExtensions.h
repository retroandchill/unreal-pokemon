// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Input/UIActionBindingHandle.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "ActionRouterExtensions.generated.h"

class UCommonActivatableWidget;
class UCommonUIActionRouterBase;
struct FCSBindUIActionArgs;
/**
 *
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UActionRouterExtensions : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

  public:
    UFUNCTION(meta = (ScriptMethod))
    static FUIActionBindingHandle RegisterUIActionBinding(UCommonUIActionRouterBase *ActionRouter,
                                                          const UWidget *Widget,
                                                          const FCSBindUIActionArgs &BindActionArgs);
};
