using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents detailed information about an ability component associated with a Pokémon.
/// </summary>
[UStruct]
public readonly partial record struct FAbilityComponentInfo
{
    /// <summary>
    /// The index of the ability associated with the Pokémon's ability component.
    /// </summary>
    /// <remarks>
    /// This property is required and is typically used to identify or reference a specific ability.
    /// It plays a crucial role in determining the behavior or functionality associated with the Pokémon's abilities.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required int AbilityIndex { get; init; }

    /// <summary>
    /// Represents an explicit override or specific configuration for a Pokémon's ability.
    /// </summary>
    /// <remarks>
    /// This property is optional and provides a means to directly specify or assign an ability handle
    /// to the Pokémon's ability component. When set, it takes precedence over the default ability behavior
    /// determined by the ability index.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required TOptional<FAbilityHandle> ExplicitAbility { get; init; }
}
