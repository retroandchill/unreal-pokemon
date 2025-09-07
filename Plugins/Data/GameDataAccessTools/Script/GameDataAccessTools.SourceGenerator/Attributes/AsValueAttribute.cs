#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Property | AttributeTargets.Field)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class AsValueAttribute : Attribute;
