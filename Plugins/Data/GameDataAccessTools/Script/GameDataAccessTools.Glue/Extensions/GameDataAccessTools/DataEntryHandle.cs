using System.Diagnostics.CodeAnalysis;

namespace UnrealSharp.GameDataAccessTools;

public partial record struct FDataHandleEntry
{
    [SetsRequiredMembers]
    public FDataHandleEntry(FName id, FText displayName)
    {
        ID = id;
        DisplayName = !displayName.IsNullOrWhitespace() ? displayName : id.ToString();
    }

    [SetsRequiredMembers]
    public FDataHandleEntry(FName id)
        : this(id, FText.None) { }

    public void Deconstruct(out FName id, out FText displayName)
    {
        id = ID;
        displayName = DisplayName;
    }
}
