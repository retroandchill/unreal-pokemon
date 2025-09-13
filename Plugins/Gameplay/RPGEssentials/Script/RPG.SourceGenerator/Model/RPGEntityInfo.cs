using System.Collections.Immutable;
using Microsoft.CodeAnalysis;

namespace RPG.SourceGenerator.Model;

public record RPGEntityInfo(INamedTypeSymbol ClassSymbol)
{
    public string Namespace => ClassSymbol.ContainingNamespace.ToDisplayString();
    public string ClassName => ClassSymbol.Name;
    public string EngineName => ClassName[1..];

    public required ImmutableArray<ComponentInfo> Components { get; init; }
    public required IMethodSymbol? FactoryMethod { get; init; }

    public bool HasFactory => FactoryMethod is not null;

    public ITypeSymbol? OuterType => FactoryMethod?.Parameters[0].Type;

    public string? OuterParam => FactoryMethod?.Parameters[0].Name;

    public ITypeSymbol? InitializerStruct => FactoryMethod?.Parameters[1].Type;

    public string? InitializerParams => FactoryMethod?.Parameters[1].Name;

    public ISymbol? SubclassSourceSymbol { get; init; }

    public string? SubclassSource => SubclassSourceSymbol?.Name;

    public bool SubclassSourceIsMethod => SubclassSourceSymbol is IMethodSymbol;

    public bool HasSubclassSource => SubclassSource is not null;
}
