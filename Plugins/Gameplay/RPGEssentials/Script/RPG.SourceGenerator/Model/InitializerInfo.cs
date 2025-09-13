using Microsoft.CodeAnalysis;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;
using RPG.SourceGenerator.Attributes;

namespace RPG.SourceGenerator.Model;

[AttributeInfoType<InitializerAttribute>]
public readonly record struct InitializerInfo(string InitializerName);
