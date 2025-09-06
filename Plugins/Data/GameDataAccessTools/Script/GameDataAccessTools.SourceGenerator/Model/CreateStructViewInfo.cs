using GameAccessTools.SourceGenerator.Attributes;
using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType(typeof(CreateStructViewAttribute<>))]
public readonly record struct CreateStructViewInfo(ITypeSymbol Type);
