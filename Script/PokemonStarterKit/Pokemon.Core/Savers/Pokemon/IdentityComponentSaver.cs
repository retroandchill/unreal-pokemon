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
[ComponentSaver<UIdentityComponent, FIdentityComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UIdentityComponentSaver : UComponentSaver
{
    private partial FIdentityComponentInfo SaveComponent(UIdentityComponent component);

    [MapperIgnoreTarget(nameof(UIdentityComponent.InitFunction))]
    private partial void LoadComponent(FIdentityComponentInfo info, UIdentityComponent component);
}
