using InteractiveUI.Core;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// UItemInfoWidgetBase is a base class for widgets that display information about a specific item.
/// It extends UCommonUserWidget and implements the IRefreshable interface to support item-specific updates.
/// The class provides functionality for setting and updating the current item,
/// and for performing any necessary visual or logical updates when the item changes.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UItemInfoWidgetBase : UCommonUserWidget, IRefreshable
{
    /// <summary>
    /// Represents the item associated with the widget, encapsulated as an FItemHandle.
    /// When this property is set, the widget updates its display or logic based on the new item value.
    /// Assigning a new value to this property triggers the Refresh() method to apply necessary changes.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Content")]
    public FItemHandle Item
    {
        get;
        set
        {
            if (field == value)
                return;

            field = value;
            Refresh();
        }
    }

    /// <inheritdoc />
    public void Refresh()
    {
        if (Item.IsValid)
        {
            OnItemSet(Item);
        }
    }

    /// <summary>
    /// Called when the item is set or updated in the widget.
    /// Provides the ability to handle any logic or display updates required
    /// when the underlying item data changes.
    /// </summary>
    /// <param name="item">The handle representing the item that was set.</param>
    [UFunction(FunctionFlags.BlueprintEvent, Category = "Display")]
    protected virtual void OnItemSet(FItemHandle item) { }
}
