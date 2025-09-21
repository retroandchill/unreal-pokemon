using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;

namespace InteractiveUI.Core.Selection;

[UClass(ClassFlags.Abstract)]
public class USimpleTextButtonBase : UCommonButtonBase
{
    [UProperty]
    [BindWidget]
    private UCommonTextBlock TextBlock { get; }

    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite, Category = "Display")]
    public FText Text
    {
        get;
        set
        {
            field = value;

#if WITH_EDITOR
            if (!SystemLibrary.IsValid(TextBlock))
                return;
#endif
            TextBlock.Text = value;
        }
    }

    public override void PreConstruct(bool isDesignTime)
    {
#if WITH_EDITOR
        if (!SystemLibrary.IsValid(TextBlock))
            return;
#endif
        
        TextBlock.Text = Text;
    }

    protected override void OnCurrentTextStyleChanged()
    {
#if WITH_EDITOR
        if (!SystemLibrary.IsValid(TextBlock))
            return;
#endif
        TextBlock.Style = CurrentTextStyleClass;
    }
}
