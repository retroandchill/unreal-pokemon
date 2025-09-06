namespace UnrealSharp.GameDataAccessTools;

public partial class UStaticGameDataRepository
{
    protected void RegisterEntryInternal<T>(T entry)
        where T : MarshalledStruct<T>
    {
        if (!TryRegisterEntryInternal(entry))
            throw new InvalidOperationException($"Invalid entry '{entry}' passed to repository");
    }

    public void UnregisterEntry(FName id)
    {
        if (!TryUnregisterEntry(id))
            throw new InvalidOperationException($"No entry with ID '{id}' found in repository");
    }
}
