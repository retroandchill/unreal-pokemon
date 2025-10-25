using LanguageExt;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents the component metadata for a holdable item attached to a Pokémon in the game.
/// </summary>
[UStruct]
public readonly partial record struct FHoldItemComponentInfo
{
    /// <summary>
    /// Gets or sets the holdable item associated with a Pokémon.
    /// This property represents an optional item that is attached to a Pokémon
    /// and may provide additional effects or abilities during gameplay.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required Option<FItemHandle> Item { get; init; }
}
