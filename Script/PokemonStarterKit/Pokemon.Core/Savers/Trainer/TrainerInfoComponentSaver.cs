using Pokemon.Core.Components.Trainer;
using Pokemon.Core.Model.Trainer;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Trainer;

[UClass]
[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
[ComponentSaver<UTrainerInfoComponent, FTrainerInfoComponentInfo>(
    LoggerClass = typeof(LogPokemonCore)
)]
public partial class UTrainerInfoComponentSaver : UComponentSaver
{
    private partial FTrainerInfoComponentInfo SaveComponent(UTrainerInfoComponent component);

    [MapperIgnoreTarget(nameof(UTrainerInfoComponent.InitFunction))]
    private partial void LoadComponent(
        FTrainerInfoComponentInfo info,
        UTrainerInfoComponent component
    );
}
