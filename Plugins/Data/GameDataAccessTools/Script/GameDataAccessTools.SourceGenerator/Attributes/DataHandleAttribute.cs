#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal abstract class DataHandleBaseAttribute : Attribute;

[AttributeUsage(AttributeTargets.Struct)]
internal class DataHandleAttribute(Type provider, string repositoryName) : DataHandleBaseAttribute
{
    public Type Provider { get; } = provider;
    public string RepositoryName { get; } = repositoryName;
}

[AttributeUsage(AttributeTargets.Struct)]
internal class DataHandleAttribute<T> : DataHandleBaseAttribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct
#else
    where T : struct, IGameDataEntry, MarshalledStruct<T>
#endif
{ }
