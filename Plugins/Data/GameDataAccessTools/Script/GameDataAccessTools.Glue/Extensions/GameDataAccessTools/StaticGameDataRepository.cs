namespace UnrealSharp.GameDataAccessTools;

public partial class UStaticGameDataRepository
{
    /// <summary>
    /// Registers an entry of the specified type into the static game data repository.
    /// Throws an <see cref="InvalidOperationException"/> if the entry fails to register.
    /// </summary>
    /// <typeparam name="T">The type of the entry being registered, which must inherit from <see cref="MarshalledStruct{T}"/>.</typeparam>
    /// <param name="entry">The entry to be registered.</param>
    protected void RegisterEntryInternal<T>(T entry)
        where T : MarshalledStruct<T>
    {
        if (!TryRegisterEntryInternal(out var error, entry))
            throw new InvalidOperationException(error);
    }

    /// <summary>
    /// Unregisters an entry from the static game data repository using the specified identifier.
    /// Throws an <see cref="InvalidOperationException"/> if no entry with the specified identifier is found.
    /// </summary>
    /// <param name="id">The unique identifier of the entry to be unregistered.</param>
    public void UnregisterEntry(FName id)
    {
        if (!TryUnregisterEntry(id))
            throw new InvalidOperationException($"No entry with ID '{id}' found in repository");
    }
}
