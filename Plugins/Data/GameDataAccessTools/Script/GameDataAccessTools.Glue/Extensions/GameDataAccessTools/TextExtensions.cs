namespace UnrealSharp.GameDataAccessTools;

/// <summary>
/// Provides extension methods for text-related operations on the FText type.
/// </summary>
public static class TextExtensions
{
    /// <summary>
    /// Checks if the specified FText object is null, has an empty value,
    /// represents a "None" value, or consists entirely of whitespace characters.
    /// </summary>
    /// <param name="text">The FText object to check for null, empty, or whitespace content.</param>
    /// <returns>
    /// True if the FText object is null, empty, represents "None", or contains only whitespace.
    /// Otherwise, returns false.
    /// </returns>
    public static bool IsNullOrWhitespace(this FText? text)
    {
        return text is null || text.Empty || text == FText.None || text.AsReadOnlySpan().IsWhiteSpace();
    }
}
