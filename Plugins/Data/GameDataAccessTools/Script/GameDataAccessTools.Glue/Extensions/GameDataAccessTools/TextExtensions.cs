namespace UnrealSharp.GameDataAccessTools;

public static class TextExtensions
{
    public static bool IsNullOrWhitespace(this FText? text)
    {
        return text is null
            || text.Empty
            || text == FText.None
            || text.AsReadOnlySpan().IsWhiteSpace();
    }
}
