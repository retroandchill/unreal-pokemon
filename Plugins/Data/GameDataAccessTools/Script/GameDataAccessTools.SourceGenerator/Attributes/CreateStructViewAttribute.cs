#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

[AttributeUsage(AttributeTargets.Struct)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class CreateStructViewAttribute : Attribute;

[AttributeUsage(AttributeTargets.Assembly, AllowMultiple = true)]
internal class CreateStructViewAttribute<T> : Attribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct;
#else
    where T : struct, IGameDataEntry, MarshalledStruct<T>;
#endif
