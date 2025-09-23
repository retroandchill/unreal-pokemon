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
[ComponentSaver<UStatusEffectComponent, FStatusEffectComponentInfo>(
    LoggerClass = typeof(LogPokemonCore)
)]
public partial class UStatusEffectComponentSaver : UComponentSaver
{
    private partial FStatusEffectComponentInfo SaveComponent(UStatusEffectComponent component);

    [MapperIgnoreTarget(nameof(UStatusEffectComponent.InitFunction))]
    private partial void LoadComponent(
        FStatusEffectComponentInfo info,
        UStatusEffectComponent component
    );
}
