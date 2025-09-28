#if POKEMON_GENERATOR
using RhoMicro.CodeAnalysis;
#endif

namespace Pokemon.SourceGenerators.Attributes;

[AttributeUsage(AttributeTargets.Method)]
#if POKEMON_GENERATOR
[IncludeFile]
#endif
internal class BattleAbilityEventAttribute(string eventName, string abilityId) : Attribute
{
    public string EventName { get; } = eventName;

    public string AbilityID { get; } = abilityId;
}
