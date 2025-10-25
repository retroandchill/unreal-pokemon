using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;

namespace GameDataAccessTools.Core;

/// <summary>
/// Represents a generic data handle interface providing access to structured data
/// and its associated metadata within the application. Defines common properties
/// and behaviors for all data handle types to enable consistent data access and
/// validation mechanisms.
/// </summary>
public interface IDataHandle : IEquatable<FName>
{
    /// <summary>
    /// Represents a static property that provides access to the structure definition
    /// of an entry within the associated data repository or data context.
    /// This property serves as a blueprint defining the schema for individual data
    /// entries managed by the system.
    /// </summary>
    static abstract UScriptStruct EntryStruct { get; }

    /// <summary>
    /// Represents a static property that provides access to a collection
    /// of data handle entries associated with a specific data context or type.
    /// This property is used to enumerate all entries available for a given
    /// data handle implementation, enabling structured access and management
    /// of the respective data.
    /// </summary>
    static abstract IEnumerable<FDataHandleEntry> Entries { get; }

    /// <summary>
    /// Represents the unique identifier associated with the data handle or entry.
    /// This property is utilized to uniquely reference and differentiate individual
    /// data entries within the broader data handling context.
    /// </summary>
    FName ID { get; }

    /// <summary>
    /// Gets a value indicating whether the current instance is valid and
    /// properly structured according to the expected data definitions.
    /// This property is used to identify if the instance can reliably
    /// participate in operations or comply with constraints within the system.
    /// </summary>
    bool IsValid { get; }
}

/// <summary>
/// Defines a contract for a data handle that represents a unique, identifiable
/// and structured data entry within the system.
/// This interface includes mechanisms to access the associated data ID,
/// verify its validity, and provides metadata or entries associated with the data structure.
/// It also supports equality comparison for identification purposes.
/// </summary>
public interface IDataHandle<T> : IDataHandle
    where T : struct, IGameDataEntry, MarshalledStruct<T>
{
    /// <summary>
    /// Provides access to a struct representation of a data entry in the underlying system.
    /// This property allows structured and type-safe access to the associated data entry,
    /// facilitating interaction with and manipulation of the data managed by the system.
    /// </summary>
    StructView<T> Entry { get; }
}
