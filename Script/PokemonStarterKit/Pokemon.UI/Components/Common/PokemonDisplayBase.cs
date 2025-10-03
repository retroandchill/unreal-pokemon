using Pokemon.Core.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Represents a base class for displaying Pokémon-related data in the UI.
/// This class is abstract and serves as a foundation for more specific Pokémon display widgets.
/// Inherits from UCommonUserWidget.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UPokemonDisplayBase : UCommonUserWidget
{
    /// <summary>
    /// Represents a property of type <see cref="UPokemon"/> used within the Pokémon display components.
    /// This property is commonly utilized to get or set the Pokémon entity that should be represented
    /// in the respective user interface components, such as panels or displays.
    /// </summary>
    /// <remarks>
    /// When the property value is set, the corresponding display or panel invokes the <c>Refresh</c> method
    /// to update the UI based on the newly assigned Pokémon entity.
    /// </remarks>
    /// <value>
    /// The <see cref="UPokemon"/> entity associated with this component.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Content")]
    public UPokemon? Pokemon
    {
        get;
        set
        {
            if (ReferenceEquals(field, value))
                return;

            field = value;
            Refresh();
        }
    }

    /// <summary>
    /// Refreshes the display with the current Pokémon data, updating relevant UI components.
    /// This method should be overridden in derived classes to implement specific refresh functionality based on the display type.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void Refresh()
    {
        // No native implementation
    }
}
