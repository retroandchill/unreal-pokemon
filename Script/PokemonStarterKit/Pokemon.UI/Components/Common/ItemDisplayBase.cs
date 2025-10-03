using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Represents a base class for displaying item-related information in the UI.
/// This class is designed to be inherited by more specific item display components.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UItemDisplayBase : UCommonUserWidget
{
    /// <summary>
    /// Represents a handle to an item in the Pokémon game's data model. This property is utilized for managing and displaying
    /// item-related data within the UI components. Changes to this property trigger the appropriate refresh or update of the UI.
    /// </summary>
    /// <remarks>
    /// The `Item` property is an instance of `FItemHandle`, which provides a reference to specific items defined
    /// within the game's data repository. This property serves as the core linkage among several components
    /// that manage item data, visual representation, and interactions in the UI.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Content")]
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

    /// <summary>
    /// Refreshes the display of the item-related component.
    /// This method is responsible for updating the UI representation
    /// based on the current state of the associated item.
    /// </summary>
    /// <remarks>
    /// Typically called to reflect changes in the item, such as updates to its properties
    /// or when the item is set or cleared. The method is implemented by derived classes
    /// to define specific behavior for refreshing the display.
    /// </remarks>
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void Refresh() { }
}
