// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Groups/CommonWidgetGroupBase.h"

#include "CSCommonWidgetGroupBase.generated.h"

/**
 * UCSCommonWidgetGroupBase class is derived from UCommonWidgetGroupBase and provides additional functionality
 * to handle widget addition, removal, and the clearing of all widgets. It also exposes events for Blueprint
 * implementation.
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UCSCommonWidgetGroupBase : public UCommonWidgetGroupBase
{
    GENERATED_BODY()

  protected:
    void OnWidgetAdded(UWidget *NewWidget) override;
    void OnWidgetRemoved(UWidget *RemovedWidget) override;
    void OnRemoveAll() override;

    /**
     * Blueprint-implementable event triggered when a widget is added to the widget group.
     *
     * @param Button The widget that has been added to the group.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Added", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetAdded"))
    void K2_OnWidgetAdded(UWidget *Button);

    /**
     * Blueprint-implementable event triggered when a widget is removed from the widget group.
     *
     * @param Button The widget that has been removed from the group.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Removed", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetRemoved"))
    void K2_OnWidgetRemoved(UWidget *Button);

    /**
     * Blueprint-implementable event triggered when all widgets are removed from the widget group.
     * This event can be used in Blueprints to handle custom logic when the group is cleared.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Remove All", Category = "ButtonGroup",
              meta = (ScriptName = "OnRemoveAll"))
    void K2_OnRemoveAll();
};
