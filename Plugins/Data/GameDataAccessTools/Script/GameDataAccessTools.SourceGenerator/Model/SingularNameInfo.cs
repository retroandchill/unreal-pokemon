using GameAccessTools.SourceGenerator.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType<SingularNameAttribute>]
public readonly record struct SingularNameInfo(string Name);
