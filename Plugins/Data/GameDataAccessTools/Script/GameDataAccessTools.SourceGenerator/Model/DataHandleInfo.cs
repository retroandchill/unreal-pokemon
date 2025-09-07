using System.Diagnostics.CodeAnalysis;
using GameAccessTools.SourceGenerator.Attributes;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType<DataHandleBaseAttribute>]
public record DataHandleBaseInfo
{
    public ITypeSymbol[] ComparableTypes { get; init; } = [];
}

[AttributeInfoType<DataHandleAttribute>]
public record DataHandleInfo(ITypeSymbol Type, string RepositoryName) : DataHandleBaseInfo;

[AttributeInfoType(typeof(DataHandleAttribute<>))]
public record DataHandleInfoOneParam(ITypeSymbol EntryType) : DataHandleBaseInfo;
