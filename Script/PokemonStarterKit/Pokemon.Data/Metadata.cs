namespace Pokemon.Data;

/// <summary>
/// Provides constant metadata definitions that can be utilized as string identifiers
/// for various attributes or properties within the application.
/// </summary>
public static class Metadata
{
    /// <summary>
    /// Represents a constant metadata identifier "AllowNone".
    /// This metadata tag allows an associated property to support a "none" value,
    /// enabling the configuration of optional fields where no specific value is required.
    /// Commonly used in scenarios involving conditional fields such as
    /// item, move, or species-related properties.
    /// </summary>
    public const string AllowNone = "AllowNone";
}
