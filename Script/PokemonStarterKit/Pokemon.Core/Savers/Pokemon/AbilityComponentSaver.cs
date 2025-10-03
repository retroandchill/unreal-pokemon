using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// UAbilityComponentSaver is responsible for saving and retrieving the state of <see cref="UAbilityComponent"/>
/// instances. It acts as a component saver that maps objects of <see cref="UAbilityComponent"/> to
/// <see cref="FAbilityComponentInfo"/> based on a specified mapping strategy.
/// </summary>
/// <remarks>
/// This class utilizes the Mapperly for automated mapping and can log using the <see cref="LogPokemonCore"/> class.
/// It is part of the RPG saving infrastructure and extends the <see cref="UComponentSaver"/>, which in turn derives
/// from <see cref="USaverBase"/>. It is specifically tailored for handling components related to abilities in the
/// Pokemon.Core namespace.
/// </remarks>
/// <seealso cref="UAbilityComponent"/>
/// <seealso cref="FAbilityComponentInfo"/>
/// <seealso cref="UComponentSaver"/>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UAbilityComponent, FAbilityComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UAbilityComponentSaver : UComponentSaver
{
    private partial FAbilityComponentInfo SaveComponent(UAbilityComponent component);

    [MapperIgnoreTarget(nameof(UAbilityComponent.InitFunction))]
    [MapperIgnoreTarget(nameof(UAbilityComponent.Ability))]
    private partial void LoadComponent(FAbilityComponentInfo info, UAbilityComponent component);
}
