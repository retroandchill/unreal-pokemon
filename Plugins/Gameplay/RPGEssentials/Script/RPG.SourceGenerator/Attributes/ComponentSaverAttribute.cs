#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#else
using UnrealSharp;
using UnrealSharp.RPGCore;
#endif

namespace RPG.SourceGenerator.Attributes;

#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Class)]
public class ComponentSaverAttribute<TComponent, TModel> : Attribute
#if RPG_ESSENTIALS_GENERATOR
    where TModel : struct
#else
    where TComponent : URPGComponent
    where TModel : struct, MarshalledStruct<TModel>
#endif
{
    public Type? LoggerClass { get; init; }
}
