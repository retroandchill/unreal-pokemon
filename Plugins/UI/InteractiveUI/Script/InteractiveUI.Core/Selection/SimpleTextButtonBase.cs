using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Core;
using UnrealSharp.Engine;

namespace InteractiveUI.Core.Selection;

/// <summary>
/// Represents a simple text button base class that extends functionality from
/// UCommonButtonBase. This class provides the ability to manage and display
/// a text property alongside common button features.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class USimpleTextButtonBase : UCommonButtonBase
{
    [UProperty]
    [BindWidget]
    private partial UCommonTextBlock TextBlock { get; }

    /// <summary>
    /// Gets or sets the text associated with the button. This property is used for setting
    /// the displayed text content of the button's associated text block.
    /// </summary>
    /// <remarks>
    /// The value of this text is automatically synchronized with the UI element represented
    /// by the button's text block. When this property is updated, the displayed text on the
    /// button updates accordingly.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite,
        Category = "Display",
        BlueprintAccessors = true
    )]
    public FText Text
    {
        get => Text_BackingField;
        set
        {
            Text_BackingField = value;

#if WITH_EDITOR
            if (!SystemLibrary.IsValid(TextBlock))
                return;
#endif
            TextBlock.Text = value;
        }
    }

    /// <inheritdoc />
    protected override void PreConstruct_Implementation(bool isDesignTime)
    {
#if WITH_EDITOR
        if (!SystemLibrary.IsValid(TextBlock))
            return;
#endif
        TextBlock.Text = Text;
    }

    /// <inheritdoc />
    protected override void OnCurrentTextStyleChanged_Implementation()
    {
#if WITH_EDITOR
        if (!SystemLibrary.IsValid(TextBlock))
            return;
#endif
        TextBlock.Style = CurrentTextStyleClass;
    }
}
