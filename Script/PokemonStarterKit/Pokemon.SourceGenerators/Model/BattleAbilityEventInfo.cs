using Pokemon.SourceGenerators.Attributes;
using Retro.SourceGeneratorUtilities.Utilities.Attributes;

namespace Pokemon.SourceGenerators.Model;

[AttributeInfoType<BattleAbilityEventAttribute>]
public readonly record struct BattleAbilityEventInfo(string EventName, string AbilityID);
