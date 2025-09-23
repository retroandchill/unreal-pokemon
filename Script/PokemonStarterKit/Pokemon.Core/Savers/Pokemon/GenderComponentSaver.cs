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
[ComponentSaver<UGenderComponent, FGenderComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UGenderComponentSaver : UComponentSaver
{
    private partial FGenderComponentInfo SaveComponent(UGenderComponent component);

    [MapperIgnoreTarget(nameof(UGenderComponent.InitFunction))]
    private partial void LoadComponent(FGenderComponentInfo info, UGenderComponent component);
}
