// "Unreal Pokémon" created by Retro & Chill.

#pragma once

#include "CoreMinimal.h"
#include "Groups/CommonButtonGroupBase.h"

#include "CSCommonButtonGroupBase.generated.h"

/**
 * UCSCommonButtonGroupBase class that extends UCommonButtonGroupBase.
 * This class provides additional functionality for handling widgets in a button group.
 * It includes overrides for adding, removing, and removing all widgets,
 * along with Blueprint-implementable events allowing further customization in Blueprints.
 */
UCLASS()
class UNREALSHARPCOMMONUI_API UCSCommonButtonGroupBase : public UCommonButtonGroupBase
{
    GENERATED_BODY()

  protected:
    void OnWidgetAdded(UWidget *NewWidget) override;
    void OnWidgetRemoved(UWidget *RemovedWidget) override;
    void OnRemoveAll() override;

    /**
     * Blueprint-implementable event triggered when a new widget is added to the button group.
     * This function allows further customization in Blueprints when a widget of type UCommonButtonBase is added.
     *
     * @param Button The button widget that was added to the button group, of type UCommonButtonBase.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Added", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetAdded"))
    void K2_OnWidgetAdded(UCommonButtonBase *Button);

    /**
     * Blueprint-implementable event triggered when a widget is removed from the button group.
     * This function allows further customization in Blueprints when a widget of type UCommonButtonBase is removed.
     *
     * @param Button The button widget that was removed from the button group, of type UCommonButtonBase.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Widget Removed", Category = "ButtonGroup",
              meta = (ScriptName = "OnWidgetRemoved"))
    void K2_OnWidgetRemoved(UCommonButtonBase *Button);

    /**
     * Blueprint-implementable event triggered when all widgets are removed from the button group.
     * This function allows further customization in Blueprints for handling actions or logic
     * when the button group is cleared of all widgets.
     */
    UFUNCTION(BlueprintImplementableEvent, DisplayName = "On Remove All", Category = "ButtonGroup",
              meta = (ScriptName = "OnRemoveAll"))
    void K2_OnRemoveAll();
};
