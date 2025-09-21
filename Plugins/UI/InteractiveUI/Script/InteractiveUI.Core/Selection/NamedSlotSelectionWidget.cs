using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

[UClass(ClassFlags.Abstract)]
public class UNamedSlotSelectionWidget : USelectionWidget, IRefreshable
{
    [UProperty]
    [BindWidget]
    private UNamedSlot ContentSlot { get; }

    public override void Construct()
    {
        base.Construct();
        UpdateOptions();
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Selection")]
    public void UpdateOptions()
    {
        #if WITH_EDITOR
        if (!SystemLibrary.IsValid(ContentSlot)) return;
        #endif

        if (ContentSlot.Content is not UPanelWidget contentWidget)
        {
            LogInteractiveUICore.LogError("ContentSlot contain a UPanelWidget");
            return;
        }
        
        Buttons.RemoveAll();
        for (var i = 0; i < contentWidget.ChildrenCount; i++)
        {
            var child = contentWidget.GetChildAt(i);
            if (child is not UCommonButtonBase button)
            {
                continue;
            }

            if (child is IConditionalWidget conditionalWidget)
            {
                conditionalWidget.UpdateEnabled();
                if (!conditionalWidget.IsWidgetEnabled) continue;
            }
            
            Buttons.AddWidget(button);
        }
        
        DesiredFocusIndex = Math.Clamp(DesiredFocusIndex, 0, Buttons.ButtonCount - 1);
    }

    public void Refresh()
    {
        UpdateOptions();
    }
}