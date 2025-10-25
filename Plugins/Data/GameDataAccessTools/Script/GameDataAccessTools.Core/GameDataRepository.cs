using System.Diagnostics.CodeAnalysis;
using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;

namespace GameDataAccessTools.Core;

/// Represents a repository interface for managing access to game data entries.
/// Provides methods for retrieving, validating, and working with structured game data.
/// Type Parameter:
/// T: The type of the game data entry. Must be a value type that implements IGameDataEntry and MarshalledStruct<T>.
public interface IGameDataRepository<T>
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    /// <summary>
    /// Gets the native pointer associated with the game data repository.
    /// This property provides access to the underlying unmanaged object, allowing
    /// advanced operations that may require direct interaction with the native representation.
    /// Typically utilized for low-level or performance-critical operations.
    /// </summary>
    IntPtr NativeObject { get; }

    /// <summary>
    /// Retrieves the underlying UScriptStruct definition associated with the game data entry type.
    /// This property identifies the structured representation used for marshaling and managing
    /// game data within the repository. It serves as a critical link between the managed and
    /// unmanaged data layers, ensuring consistency and type safety during data operations.
    /// </summary>
    UScriptStruct EntryStruct { get; }

    /// <summary>
    /// Provides access to the collection of game data entries. This property
    /// represents a view of the unmanaged array of entries, enabling safe and efficient
    /// interaction with the underlying data structures.
    /// The collection contains individual structured elements, offering functionality
    /// to examine or manipulate game data entries in a controlled, low-level manner.
    /// </summary>
    ArrayView<StructView<T>> Entries { get; }

    /// <summary>
    /// Gets the total number of entries in the repository.
    /// This property represents the count of all game data entries available
    /// within the repository, providing an efficient means to determine
    /// the size of the dataset managed by the repository.
    /// </summary>
    int NumEntries { get; }

    /// <summary>
    /// Validates if the provided identifier corresponds to a valid game data entry.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to validate.</param>
    /// <returns>True if the identifier matches a valid game data entry; otherwise, false.</returns>
    bool IsValidEntry(FName id);

    /// <summary>
    /// Retrieves the game data entry corresponding to the provided identifier.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to retrieve.</param>
    /// <returns>A StructView instance containing the game data entry associated with the specified identifier.</returns>
    StructView<T> GetEntry(FName id);

    /// <summary>
    /// Retrieves the game data entry corresponding to the specified identifier.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to retrieve.</param>
    /// <returns>A <see cref="StructView{T}"/> representing the game data entry associated with the specified identifier.</returns>
    StructView<T> GetEntry(int index);

    /// <summary>
    /// Attempts to retrieve the game data entry associated with the specified identifier.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to retrieve.</param>
    /// <param name="entry">When this method returns, contains the game data entry corresponding to the given identifier if found; otherwise, an uninitialized value.</param>
    /// <returns>True if the entry is successfully retrieved; otherwise, false.</returns>
    bool TryGetEntry(FName id, out StructView<T> entry);

    /// <summary>
    /// Attempts to retrieve a game data entry by its index.
    /// </summary>
    /// <param name="index">The zero-based index of the game data entry to retrieve.</param>
    /// <param name="entry">
    /// When this method returns, contains the game data entry if the retrieval is successful; otherwise, contains the default value.
    /// </param>
    /// <returns>True if the game data entry is successfully retrieved; otherwise, false.</returns>
    bool TryGetEntry(int index, out StructView<T> entry);
}

/// <summary>
/// Represents a repository interface for managing static access to game data entries with additional
/// support for registering and unregistering data entries dynamically.
/// Extends the functionality of IGameDataRepository for static game data management.
/// </summary>
/// <tparam name="T">The type of the game data entry. Must be a value type that implements <see cref="IGameDataEntry"/> and <see cref="MarshalledStruct{Self}"/>.</tparam>
public interface IStaticGameDataRepository<T> : IGameDataRepository<T>
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    /// <summary>
    /// Registers a new game data entry into the repository.
    /// </summary>
    /// <param name="entry">The game data entry to register.</param>
    void RegisterEntry(T entry);

    /// <summary>
    /// Attempts to register a game data entry into the repository.
    /// </summary>
    /// <param name="entry">The game data entry to register.</param>
    /// <returns>True if the entry was successfully registered; otherwise, false.</returns>
    bool TryRegisterEntry(T entry);

    /// <summary>
    /// Unregisters a game data entry from the repository.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to unregister.</param>
    void UnregisterEntry(FName id);

    /// <summary>
    /// Attempts to unregister a game data entry from the repository using the specified identifier.
    /// </summary>
    /// <param name="id">The identifier of the game data entry to attempt to unregister.</param>
    /// <returns>True if the game data entry was successfully unregistered; otherwise, false.</returns>
    bool TryUnregisterEntry(FName id);
}
