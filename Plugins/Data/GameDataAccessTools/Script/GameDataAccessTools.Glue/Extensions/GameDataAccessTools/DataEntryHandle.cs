using System.Diagnostics.CodeAnalysis;

namespace UnrealSharp.GameDataAccessTools;

public partial record struct FDataHandleEntry
{
    [SetsRequiredMembers]
    public FDataHandleEntry(FName id, FText displayName)
    {
        ID = id;
        DisplayName = !IsEmptyText(displayName) ? displayName : id.ToString();
    }
    
    [SetsRequiredMembers]
    public FDataHandleEntry(FName id) : this(id, FText.None)
    {
    }

    private static bool IsEmptyText(FText text)
    {
        return text.Empty || text == FText.None || text.AsReadOnlySpan().IsWhiteSpace();
    }
}