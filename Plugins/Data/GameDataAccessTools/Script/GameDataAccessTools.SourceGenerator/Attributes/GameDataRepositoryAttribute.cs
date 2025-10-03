#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// An attribute used to mark a class as a game data repository with a specific entry type.
/// Classes annotated with this attribute can be processed by the game data repository source generator to facilitate
/// the management and generation of related boilerplate code for handling game data entries.
/// </summary>
/// <typeparam name="T">
/// The type of the data entries managed by the repository. This type must be a value type (struct).
/// </typeparam>
[AttributeUsage(AttributeTargets.Class)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class GameDataRepositoryAttribute<T> : Attribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct;
#else
    where T : struct, MarshalledStruct<T>;
#endif
