#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Class)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class StaticIdentifierAttribute<T>(params string[] keys) : Attribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct
#else
    where T : struct, IGameDataEntry, MarshalledStruct<T>
#endif
{
    public string[] Keys { get; } = keys;
}
