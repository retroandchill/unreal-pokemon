using LanguageExt;
using Pokemon.Core.Model.Moves;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;

namespace Pokemon.Core.Model.Pokemon;

/// <summary>
/// Represents the move-related information associated with a Pokémon entity.
/// </summary>
[UStruct]
public readonly partial record struct FMoveComponentInfo
{
    /// <summary>
    /// Gets the collection of moves associated with a Pokémon.
    /// Each move is represented by an instance of <c>FPokemonMoveInfo</c>,
    /// which contains details such as the move identifier, its current Power Points (PP),
    /// and the number of PP Ups applied.
    /// </summary>
    /// <remarks>
    /// This property is marked with the <c>SaveGame</c> attribute to indicate that
    /// it should be preserved during game save and load operations. It is required
    /// to initialize this property at the time of the struct creation.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FPokemonMoveInfo> Moves { get; init; }

    /// <summary>
    /// Gets the collection of initial moves associated with a Pokémon.
    /// Each initial move is represented by an instance of <c>FMoveHandle</c>,
    /// indicating the predefined set of moves that the Pokémon can start with.
    /// </summary>
    /// <remarks>
    /// This property is marked with the <c>SaveGame</c> attribute to ensure
    /// it persists during game save and load operations. It is required to initialize
    /// this property during the creation of the struct.
    /// </remarks>
    [UProperty(PropertyFlags.SaveGame)]
    public required IReadOnlyList<FMoveHandle> InitialMoves { get; init; }
}
