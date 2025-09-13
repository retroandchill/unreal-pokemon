#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace RPG.SourceGenerator.Attributes;

#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Method)]
public class FactoryAttribute : Attribute
{
    public string? SubclassSource { get; init; }
}
