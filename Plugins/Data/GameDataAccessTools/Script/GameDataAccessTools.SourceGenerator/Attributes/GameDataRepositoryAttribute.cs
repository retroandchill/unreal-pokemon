#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Class)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
public class GameDataRepositoryAttribute<T> : Attribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct;
#else
    where T : struct, MarshalledStruct<T>;
#endif
