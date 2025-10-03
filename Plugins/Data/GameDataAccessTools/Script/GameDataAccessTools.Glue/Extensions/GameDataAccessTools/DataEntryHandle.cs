using System.Diagnostics.CodeAnalysis;

namespace UnrealSharp.GameDataAccessTools;

public partial record struct FDataHandleEntry
{
    /// <summary>
    /// Represents a data handle entry containing an identifier and a display name.
    /// </summary>
    /// <param name="id">The unique identifier for the data handle entry.</param>
    /// <param name="displayName">The display name for the data handle entry.</param>
    [SetsRequiredMembers]
    public FDataHandleEntry(FName id, FText displayName)
    {
        ID = id;
        DisplayName = !displayName.IsNullOrWhitespace() ? displayName : id.ToString();
    }

    /// <summary>
    /// Represents a data handle entry with a unique identifier and a display name.
    /// </summary>
    /// <param name="id">The unique identifier for the data handle entry.</param>
    /// <param name="displayName">The display name associated with the data handle entry.</param>
    [SetsRequiredMembers]
    public FDataHandleEntry(FName id)
        : this(id, FText.None) { }

    /// <summary>
    /// Deconstructs the FDataHandleEntry into its component fields: the identifier and display name.
    /// </summary>
    /// <param name="id">The unique identifier for the data handle entry.</param>
    /// <param name="displayName">The display name for the data handle entry.</param>
    public void Deconstruct(out FName id, out FText displayName)
    {
        id = ID;
        displayName = DisplayName;
    }
}
