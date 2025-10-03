#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// Represents the base attribute for defining a data handle structure.
/// This class serves as a foundation for attributes used to mark structures
/// that relate to data access and handle management within the context of a source generator.
/// </summary>
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal abstract class DataHandleBaseAttribute : Attribute
{
    public Type[] ComparableTypes { get; init; } = [];
}

/// <summary>
/// Defines an attribute used to mark structures as data handles in order to facilitate the
/// management of data access layers. This attribute is typically utilized by source generators
/// to identify and configure data-related structures for generation purposes.
/// </summary>
/// <param name="provider">The type of the game data provider that manages the data entry.</param>
/// <param name="repositoryName">The name of the repository that manages the data entry.</param>
[AttributeUsage(AttributeTargets.Struct)]
internal class DataHandleAttribute(Type provider, string repositoryName) : DataHandleBaseAttribute
{
    public Type Provider { get; } = provider;
    public string RepositoryName { get; } = repositoryName;
}

/// <summary>
/// Represents an attribute used to mark a structure as a data handle within the
/// context of data access and management.
/// This attribute associates the structure with a provider type and a repository name,
/// enabling functionality defined by source generators to process the marked structure.
/// </summary>
/// <typeparam name="T">The type of the game data entry. Must be a value type that implements <see cref="IGameDataEntry"/> and <see cref="MarshalledStruct{Self}"/>.</typeparam>
[AttributeUsage(AttributeTargets.Struct)]
internal class DataHandleAttribute<T> : DataHandleBaseAttribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct;
#else
    where T : struct, IGameDataEntry, MarshalledStruct<T>;
#endif
