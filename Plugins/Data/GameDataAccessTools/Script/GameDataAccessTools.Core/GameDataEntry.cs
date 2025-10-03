using UnrealSharp;

namespace GameDataAccessTools.Core;

/// <summary>
/// Represents a basic interface for game data entries, ensuring a standardized structure
/// for data models in the application.
/// </summary>
public interface IGameDataEntry
{
    /// <summary>
    /// Gets the unique identifier for the game data entry.
    /// This property is utilized across implementations of the <see cref="IGameDataEntry"/> interface
    /// to ensure consistent identification of individual data records.
    /// </summary>
    FName ID { get; }

    /// <summary>
    /// Gets the row index for the associated game data entry.
    /// This property is commonly used to uniquely identify the position of a data record
    /// within a structured dataset or collection.
    /// </summary>
    int RowIndex { get; }
}
