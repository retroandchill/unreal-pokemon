using System.Collections.Immutable;
using GameAccessTools.SourceGenerator.Attributes;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

public readonly record struct ConvertibleType(ITypeSymbol Type);

public abstract record DataHandleTemplateBase(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    ImmutableArray<ConvertibleType> Convertibles
)
{
    public long TypeVersion { get; } = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
    
    public string AssemblyName => StructType.ContainingAssembly.Name;
    public string Namespace => StructType.ContainingNamespace.ToDisplayString();
    public string StructName => StructType.Name;
    public string EngineName => StructName[1..];

    public abstract bool WithRepository { get; }

    public string? DisplayNameProperty =>
        EntryType
            .GetMembers()
            .OfType<IPropertySymbol>()
            .Where(p => p.HasAttribute<DisplayNameAttribute>())
            .Select(p => p.Name)
            .FirstOrDefault();
    public bool HasDisplayName => DisplayNameProperty is not null;
}

public sealed record ProviderTemplateHandle(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    ITypeSymbol Provider,
    string Repository,
    ImmutableArray<ConvertibleType> Convertibles
) : DataHandleTemplateBase(StructType, EntryType, Convertibles)
{
    public override bool WithRepository => true;
}

public sealed record ExplicitDataHandleTemplate(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    ImmutableArray<ConvertibleType> Convertibles
) : DataHandleTemplateBase(StructType, EntryType, Convertibles)
{
    public override bool WithRepository => false;
}
