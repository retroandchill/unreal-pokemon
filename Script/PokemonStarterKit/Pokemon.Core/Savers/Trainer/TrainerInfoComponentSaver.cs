using Pokemon.Core.Components.Trainer;
using Pokemon.Core.Model.Trainer;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Trainer;

/// <summary>
/// Represents a component saver specifically designed for the UTrainerInfoComponent type.
/// Responsible for facilitating the serialization and deserialization of trainer-related data
/// between the UTrainerInfoComponent and its associated data model FTrainerInfoComponentInfo.
/// </summary>
/// <remarks>
/// This class extends the functionality of UComponentSaver and leverages Mapperly for mapping
/// the properties of the UTrainerInfoComponent to FTrainerInfoComponentInfo. It ensures proper
/// data persistence and retrieval for trainer information during save/load operations within the game.
/// </remarks>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UTrainerInfoComponent, FTrainerInfoComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UTrainerInfoComponentSaver : UComponentSaver
{
    private partial FTrainerInfoComponentInfo SaveComponent(UTrainerInfoComponent component);

    [MapperIgnoreTarget(nameof(UTrainerInfoComponent.InitFunction))]
    private partial void LoadComponent(FTrainerInfoComponentInfo info, UTrainerInfoComponent component);
}
