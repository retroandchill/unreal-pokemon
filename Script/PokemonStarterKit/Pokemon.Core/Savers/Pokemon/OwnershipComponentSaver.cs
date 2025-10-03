using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Entities;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// Represents a component saver for the UOwnershipComponent class that facilitates serialization and deserialization processes
/// associated with ownership-related data of a Pokémon.
/// </summary>
/// <remarks>
/// This class is designed to map data between the runtime <see cref="UOwnershipComponent"/> and its corresponding
/// data structure <see cref="FOwnershipComponentInfo"/>. It uses Riok.Mapperly for efficient data mapping and
/// is marked with attributes to specify metadata for Unreal Engine integration.
/// </remarks>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UOwnershipComponent, FOwnershipComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UOwnershipComponentSaver : UComponentSaver
{
    private partial FOwnershipComponentInfo SaveComponent(UOwnershipComponent component);

    private void LoadComponent(FOwnershipComponentInfo info, UOwnershipComponent component)
    {
        LoadComponent(info, component, (UTrainer)component.OwningEntity.Outer);
    }

    [MapperIgnoreTarget(nameof(UOwnershipComponent.InitFunction))]
    private partial void LoadComponent(FOwnershipComponentInfo info, UOwnershipComponent component, UTrainer owner);
}
