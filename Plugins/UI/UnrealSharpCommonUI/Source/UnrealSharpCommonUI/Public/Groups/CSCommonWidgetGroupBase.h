// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Groups/CommonWidgetGroupBase.h"

#include "CSCommonWidgetGroupBase.generated.h"

/**
 *
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UCSCommonWidgetGroupBase : public UCommonWidgetGroupBase
{
    GENERATED_BODY()

  protected:
    void OnWidgetAdded(UWidget *NewWidget) override;
    void OnWidgetRemoved(UWidget *RemovedWidget) override;
    void OnRemoveAll() override;

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Added", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetAdded"))
    void K2_OnWidgetAdded(UWidget *Button);

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Removed", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetRemoved"))
    void K2_OnWidgetRemoved(UWidget *Button);

    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Remove All", Category = "ButtonGroup",
              meta = (ScriptName = "OnRemoveAll"))
    void K2_OnRemoveAll();
};
