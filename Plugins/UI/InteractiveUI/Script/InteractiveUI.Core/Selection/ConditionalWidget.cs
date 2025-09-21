using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace InteractiveUI.Core.Selection;

public interface IConditionalWidget
{
    bool IsWidgetEnabled { get; }
    
    void UpdateEnabled();
}

[USingleDelegate]
public delegate bool IsWidgetEnabled();

[UClass(ClassFlags.Abstract)]
public class UConditionalButtonBase : UCommonButtonBase, IConditionalWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Selection")]
    public bool IsWidgetEnabled { get; private set; } = true;
    
    [UProperty]
    private TDelegate<IsWidgetEnabled> IsWidgetEnabledDelegate { get; }
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility EnabledVisibility { get; set; } = ESlateVisibility.SelfHitTestInvisible;
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility DisabledVisibility { get; set; } = ESlateVisibility.Collapsed;

    public void UpdateEnabled()
    {
        if (IsWidgetEnabledDelegate.IsBound)
        {
            IsWidgetEnabled = IsWidgetEnabledDelegate.InnerDelegate.Invoke();
        }
        
        Visibility = IsWidgetEnabled ? EnabledVisibility : DisabledVisibility;
    }
}

[UClass(ClassFlags.Abstract)]
public class UConditionalTextButtonBase : USimpleTextButtonBase, IConditionalWidget
{
    [UProperty(PropertyFlags.EditAnywhere, Category = "Selection")]
    public bool IsWidgetEnabled { get; private set; } = true;
    
    [UProperty]
    private TDelegate<IsWidgetEnabled> IsWidgetEnabledDelegate { get; }
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility EnabledVisibility { get; set; } = ESlateVisibility.SelfHitTestInvisible;
    
    [UProperty(PropertyFlags.EditAnywhere, Category = "Visibility")]
    [UMetaData("InvalidEnumValues", "Collapsed, Hidden")]
    private ESlateVisibility DisabledVisibility { get; set; } = ESlateVisibility.Collapsed;

    public void UpdateEnabled()
    {
        if (IsWidgetEnabledDelegate.IsBound)
        {
            IsWidgetEnabled = IsWidgetEnabledDelegate.InnerDelegate.Invoke();
        }
        
        Visibility = IsWidgetEnabled ? EnabledVisibility : DisabledVisibility;
    }
}