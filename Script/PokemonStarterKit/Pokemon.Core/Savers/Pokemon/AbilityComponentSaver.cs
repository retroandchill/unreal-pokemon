using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

[UClass]
[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
[ComponentSaver<UAbilityComponent, FAbilityComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UAbilityComponentSaver : UComponentSaver
{
    private partial FAbilityComponentInfo SaveComponent(UAbilityComponent component);

    [MapperIgnoreTarget(nameof(UAbilityComponent.InitFunction))]
    [MapperIgnoreTarget(nameof(UAbilityComponent.Ability))]
    private partial void LoadComponent(FAbilityComponentInfo info, UAbilityComponent component);
}
