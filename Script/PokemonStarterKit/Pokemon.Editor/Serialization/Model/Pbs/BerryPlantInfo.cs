using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

/// <summary>
/// Represents an integer range defined by a minimum and maximum value.
/// </summary>
/// <param name="Min">The minimum value of the range.</param>
/// <param name="Max">The maximum value of the range.</param>
/// <remarks>
/// This struct is immutable and can be used to define bounds or ranges for integer values.
/// </remarks>
public readonly record struct IntBounds([property: PbsRange<int>(0)] int Min, [property: PbsRange<int>(1)] int Max);

/// <summary>
/// Represents the configuration and behavior details for a berry plant in the serialization model.
/// </summary>
/// <remarks>
/// This record includes properties for identifying the berry plant, its growth configuration, and yield.
/// It is primarily used for serialization and mapping to game data models.
/// </remarks>
public record BerryPlantInfo
{
    /// <summary>
    /// Gets the unique identifier for the berry plant.
    /// </summary>
    /// <remarks>
    /// This property holds the unique identifier for a berry plant,
    /// used to distinguish different plants in the serialization and
    /// mapping process.
    /// </remarks>
    [PbsKey]
    public FName ID { get; init; }

    /// <summary>
    /// Gets the row index of the berry plant in the serialized data.
    /// </summary>
    /// <remarks>
    /// This property identifies the ordinal position of the berry plant
    /// within its data structure, typically used to manage array or table-based
    /// serialization and deserialization processes.
    /// </remarks>
    [PbsIndex]
    public int RowIndex { get; init; }

    /// <summary>
    /// Gets the number of hours required for the berry plant to progress through each growth stage.
    /// </summary>
    /// <remarks>
    /// This property defines the duration of time, in hours, that the plant spends in each individual growth stage.
    /// It is utilized to determine the timing of the plant's development and influences the overall growth cycle.
    /// </remarks>
    [PbsRange<int>(1)]
    public int HoursPerStage { get; init; } = 3;

    /// <summary>
    /// Gets the rate at which the berry plant's soil moisture decreases per hour.
    /// </summary>
    /// <remarks>
    /// This property represents the speed at which the plant's soil dries out,
    /// measured in moisture points deducted per hour. It is used in determining
    /// the frequency of watering needed for the plant's optimal growth.
    /// </remarks>
    [PbsRange<int>(0)]
    public int DryingPerHour { get; init; } = 15;

    /// <summary>
    /// Gets the range of berries yielded by this berry plant.
    /// </summary>
    /// <remarks>
    /// This property defines the minimum and maximum number of berries that
    /// the plant can produce upon harvest. The yield range is represented by
    /// an immutable value containing these boundaries.
    /// </remarks>
    public IntBounds Yield { get; init; } = new(2, 5);
}
