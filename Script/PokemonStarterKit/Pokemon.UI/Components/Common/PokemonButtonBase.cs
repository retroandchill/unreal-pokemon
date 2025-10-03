using Pokemon.Core.Entities;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Represents a base class for UI components that display Pokémon-related information as a button.
/// </summary>
/// <remarks>
/// This class inherits from <see cref="UCommonButtonBase"/> and serves as a specialized button
/// for handling Pokémon data and interactions. It introduces functionality to bind a Pokémon instance
/// to the button and provides a method to refresh its displayed content.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public class UPokemonButtonBase : UCommonButtonBase
{
    /// <summary>
    /// Represents a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This class serves as a base class for defining a Pokémon and its associated properties.
    /// It is used as part of the core entities in the Pokémon system.
    /// </remarks>
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
    /// Refreshes the visual representation or displayed content of the button.
    /// </summary>
    /// <remarks>
    /// This method is virtual and can be overridden by derived classes to implement customized content refreshing
    /// logic. It is generally responsible for updating the button's UI elements based on the state or properties
    /// of the associated Pokémon.
    /// </remarks>
    [UFunction(FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent, Category = "Display")]
    public virtual void Refresh() { }
}
