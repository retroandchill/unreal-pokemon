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
internal class GameDataProviderAttribute : Attribute
{
    public string? DisplayName { get; init; }
    public string Category { get; init; } = "Game";
}
