#if GAME_DATA_ACCESS_TOOLS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using GameDataAccessTools.Core;
#endif

namespace GameAccessTools.SourceGenerator.Attributes;

/// <summary>
/// An attribute used to indicate that a struct should have an associated view generated.
/// </summary>
/// <remarks>
/// This attribute can be applied to structs to provide additional generated functionality or "view" capabilities.
/// </remarks>
[AttributeUsage(AttributeTargets.Struct)]
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
[IncludeFile]
#endif
internal class CreateStructViewAttribute : Attribute;

/// <summary>
/// An attribute that specifies a generic struct should have an associated view generated.
/// </summary>
/// <typeparam name="T">The type of the struct for which the view will be generated.</typeparam>
/// <remarks>
/// This attribute allows for the generation of additional functionality or data views associated with the specified struct.
/// It must be applied to a generic type constrained to a struct.
/// </remarks>
[AttributeUsage(AttributeTargets.Assembly, AllowMultiple = true)]
internal class CreateStructViewAttribute<T> : Attribute
#if GAME_DATA_ACCESS_TOOLS_GENERATOR
    where T : struct;
#else
    where T : struct, IGameDataEntry, MarshalledStruct<T>;
#endif
