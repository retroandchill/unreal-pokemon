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
[ComponentSaver<UShininessComponent, FShininessComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UShininessComponentSaver : UComponentSaver
{
    private partial FShininessComponentInfo SaveComponent(UShininessComponent component);

    [MapperIgnoreTarget(nameof(UShininessComponent.InitFunction))]
    private partial void LoadComponent(FShininessComponentInfo info, UShininessComponent component);
}
