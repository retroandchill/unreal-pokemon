using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// Handles the saving functionality for the status effect component of a Pokémon.
/// This class is responsible for persisting and restoring information related to the
/// status effects managed by a <see cref="UStatusEffectComponent"/>. It uses a mapping
/// strategy to convert the runtime data to and from a serializable data structure.
/// </summary>
/// <remarks>
/// This class extends <see cref="UComponentSaver"/> to provide specific implementation
/// details for saving the <see cref="UStatusEffectComponent"/> and its data representation
/// defined in <see cref="FStatusEffectComponentInfo"/>.
/// </remarks>
/// <seealso cref="UStatusEffectComponent"/>
/// <seealso cref="FStatusEffectComponentInfo"/>
/// <seealso cref="UComponentSaver"/>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UStatusEffectComponent, FStatusEffectComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UStatusEffectComponentSaver : UComponentSaver
{
    private partial FStatusEffectComponentInfo SaveComponent(UStatusEffectComponent component);

    [MapperIgnoreTarget(nameof(UStatusEffectComponent.InitFunction))]
    private partial void LoadComponent(FStatusEffectComponentInfo info, UStatusEffectComponent component);
}
