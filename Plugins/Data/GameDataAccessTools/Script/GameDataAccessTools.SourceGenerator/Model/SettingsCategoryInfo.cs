using GameAccessTools.SourceGenerator.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace GameAccessTools.SourceGenerator.Model;

[AttributeInfoType<SettingsCategoryAttribute>]
public record struct SettingsCategoryInfo(string Name);
