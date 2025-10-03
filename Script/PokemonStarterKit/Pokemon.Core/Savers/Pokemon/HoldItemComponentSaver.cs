using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// Represents a saver for the UHoldItemComponent, handling the serialization and deserialization
/// of component data for persistent storage.
/// This class maps to the FHoldItemComponentInfo structure for handling component state information.
/// </summary>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UHoldItemComponent, FHoldItemComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UHoldItemComponentSaver : UComponentSaver
{
    private partial FHoldItemComponentInfo SaveComponent(UHoldItemComponent component);

    [MapperIgnoreTarget(nameof(UHoldItemComponent.InitFunction))]
    private partial void LoadComponent(FHoldItemComponentInfo info, UHoldItemComponent component);
}
