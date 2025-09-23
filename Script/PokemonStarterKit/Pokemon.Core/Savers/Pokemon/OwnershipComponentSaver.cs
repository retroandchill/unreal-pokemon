using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Entities;
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
[ComponentSaver<UOwnershipComponent, FOwnershipComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UOwnershipComponentSaver : UComponentSaver
{
    private partial FOwnershipComponentInfo SaveComponent(UOwnershipComponent component);

    private void LoadComponent(FOwnershipComponentInfo info, UOwnershipComponent component)
    {
        LoadComponent(info, component, (UTrainer)component.OwningEntity.Outer);
    }

    [MapperIgnoreTarget(nameof(UOwnershipComponent.InitFunction))]
    private partial void LoadComponent(
        FOwnershipComponentInfo info,
        UOwnershipComponent component,
        UTrainer owner
    );
}
