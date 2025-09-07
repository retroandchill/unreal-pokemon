using Microsoft.CodeAnalysis;

namespace GameAccessTools.SourceGenerator.Model;

public abstract record DataHandleTemplateBase(ITypeSymbol StructType, ITypeSymbol EntryType)
{
    public string Namespace => StructType.ContainingNamespace.ToDisplayString();
    public string StructName => StructType.Name;

    public abstract bool WithRepository { get; }
}

public sealed record ProviderTemplateHandle(
    ITypeSymbol StructType,
    ITypeSymbol EntryType,
    ITypeSymbol Provider,
    string Repository
) : DataHandleTemplateBase(StructType, EntryType)
{
    public override bool WithRepository => true;
}

public sealed record ExplicitDataHandleTemplate(ITypeSymbol StructType, ITypeSymbol EntryType)
    : DataHandleTemplateBase(StructType, EntryType)
{
    public override bool WithRepository => false;
}
