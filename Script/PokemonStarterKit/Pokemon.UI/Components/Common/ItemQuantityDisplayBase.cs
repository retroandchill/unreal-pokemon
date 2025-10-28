using InteractiveUI.Core;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// An abstract base class for displaying item quantities. This class provides a framework
/// for managing the display of an item's quantity and allows customization using a text style.
/// It inherits from UCommonUserWidget and implements the IRefreshable interface.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UItemQuantityDisplayBase : UCommonUserWidget, IRefreshable
{
    /// <summary>
    /// Gets or sets the quantity of items to be displayed.
    /// </summary>
    /// <remarks>
    /// The Quantity property represents the number of items and is used to update
    /// the visual interface for displaying item quantities. It is required to be
    /// a non-negative value; otherwise, an <see cref="System.ArgumentOutOfRangeException" />
    /// will be thrown. Setting this property automatically triggers a call to the
    /// <c>Refresh</c> method to update the associated UI elements.
    /// </remarks>
    /// <exception cref="System.ArgumentOutOfRangeException">
    /// Thrown when the set value is less than 0.
    /// </exception>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Content", BlueprintAccessors = true)]
    public int Quantity
    {
        get => Quantity_BackingField;
        set
        {
            ArgumentOutOfRangeException.ThrowIfLessThan(value, 0);
            Quantity_BackingField = value;
            Refresh();
        }
    }

    /// <summary>
    /// Gets or sets the text style applied to the quantity display.
    /// </summary>
    /// <remarks>
    /// The TextStyle property determines the visual representation of the text,
    /// including font, color, size, and other style-related attributes.
    /// Changing this property triggers the <c>OnTextStyleChanged</c> method
    /// to update the associated UI elements. The value must be a valid class
    /// derived from <see cref="UCommonTextStyle" />.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite,
        Category = "Content",
        BlueprintAccessors = true
    )]
    public TSubclassOf<UCommonTextStyle> TextStyle
    {
        get => TextStyle_BackingField;
        set
        {
            if (TextStyle_BackingField == value)
                return;

            TextStyle_BackingField = value;
            OnTextStyleChanged();
        }
    }

    /// <inheritdoc />
    public virtual void Refresh()
    {
        // No implementation here
    }

    /// <summary>
    /// Invoked when the text style associated with the item quantity display changes.
    /// This method is typically used to update the visual appearance of the text
    /// in accordance with the newly applied text style.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Content")]
    protected partial void OnTextStyleChanged();

    protected virtual partial void OnTextStyleChanged_Implementation() { }
}
