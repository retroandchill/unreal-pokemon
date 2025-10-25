using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Moves;

/// <summary>
/// Represents detailed information about a Pokemon move, including its identifier,
/// current Power Points (PP), and the number of PP Ups applied to it.
/// </summary>
[UStruct]
public readonly partial record struct FPokemonMoveInfo
{
    /// <summary>
    /// Gets the handle that represents a specific Pokemon move.
    /// This handle is used to identify and reference a move within the game's data structure.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required FMoveHandle Move { get; init; }

    /// <summary>
    /// Represents the current Power Points (PP) of a Pokemon move.
    /// PP determines how many times the move can be used in battle before it is depleted.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required int PP { get; init; }

    /// <summary>
    /// Represents the number of Power Point (PP) Ups applied to a Pokemon move.
    /// PP Ups increase the maximum Power Points for the move, allowing it to be used more frequently in battles.
    /// </summary>
    [UProperty(PropertyFlags.SaveGame)]
    public required int PPUps { get; init; }
}
