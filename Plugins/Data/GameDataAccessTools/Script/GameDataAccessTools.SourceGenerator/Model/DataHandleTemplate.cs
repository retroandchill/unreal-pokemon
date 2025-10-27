using System.Collections.Immutable;
using GameAccessTools.SourceGenerator.Attributes;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using UnrealSharp.GlueGenerator;
using UnrealSharp.GlueGenerator.NativeTypes;
using UnrealSharp.GlueGenerator.NativeTypes.Properties;

namespace GameAccessTools.SourceGenerator.Model;

public readonly record struct ConvertibleType(ITypeSymbol Type);

public abstract record DataHandleTemplateBase(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    UnrealType TypeData,
    ImmutableArray<ConvertibleType> Convertibles
)
{
    public long TypeVersion { get; } = DateTimeOffset.UtcNow.ToUnixTimeSeconds();
    
    public string AssemblyName => StructType.ContainingAssembly.Name;
    public string Namespace => StructType.ContainingNamespace.ToDisplayString();
    public string StructName => StructType.Name;
    public string EngineName => StructName[1..];

    public string ModuleInitializer
    {
        get
        {
            var builder = new GeneratorStringBuilder();
            builder.BeginModuleInitializer(TypeData);
            return builder.ToString();
        }
    }

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
    UnrealType TypeData,
    ITypeSymbol Provider,
    string Repository,
    ImmutableArray<ConvertibleType> Convertibles
) : DataHandleTemplateBase(StructType, EntryType, TypeData, Convertibles)
{
    public override bool WithRepository => true;
}

public sealed record ExplicitDataHandleTemplate(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    UnrealType TypeData,
    ImmutableArray<ConvertibleType> Convertibles
) : DataHandleTemplateBase(StructType, EntryType, TypeData, Convertibles)
{
    public override bool WithRepository => false;
}
