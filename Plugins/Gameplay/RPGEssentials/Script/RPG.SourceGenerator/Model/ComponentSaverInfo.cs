using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using RPG.SourceGenerator.Attributes;

namespace RPG.SourceGenerator.Model;

[AttributeInfoType(typeof(ComponentSaverAttribute<,>))]
public record ComponentSaverInfo(ITypeSymbol ComponentClass, ITypeSymbol ModelStruct)
{
    public ITypeSymbol? LoggerClass { get; init; }
}
