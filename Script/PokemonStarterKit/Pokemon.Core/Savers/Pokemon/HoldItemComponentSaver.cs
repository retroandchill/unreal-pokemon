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
[ComponentSaver<UHoldItemComponent, FHoldItemComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UHoldItemComponentSaver : UComponentSaver
{
    private partial FHoldItemComponentInfo SaveComponent(UHoldItemComponent component);

    [MapperIgnoreTarget(nameof(UHoldItemComponent.InitFunction))]
    private partial void LoadComponent(FHoldItemComponentInfo info, UHoldItemComponent component);
}
