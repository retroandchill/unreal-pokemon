using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Model.Trainer;

/// <summary>
/// Represents detailed information about a trainer in the game, as part of the Trainer model.
/// </summary>
[UStruct]
public readonly partial record struct FTrainerInfoComponentInfo
{
    /// <summary>
    /// Represents the type of a trainer and serves as a reference to the trainer's specific characteristics or classification
    /// within the game. This property is required and used as part of the saved game data.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FTrainerTypeHandle TrainerType { get; init; }

    /// <summary>
    /// Represents the name of a trainer, typically used for identifying the trainer
    /// within the game. This property is required and serves as part of the saved game data.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents a unique identifier for a trainer within the game's data structures. This property is required and serves as part of the saved game information.
    /// </summary>
    [field: UProperty(PropertyFlags.SaveGame)]
    public required uint FullID { get; init; }
}
