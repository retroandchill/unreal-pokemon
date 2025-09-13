#if RPG_ESSENTIALS_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace RPG.SourceGenerator.Attributes;

#if RPG_ESSENTIALS_GENERATOR
[IncludeFile]
#endif
[AttributeUsage(AttributeTargets.Property)]
internal class InitializerAttribute(string initializerName) : Attribute
{
    public string InitializerName { get; } = initializerName;
}
