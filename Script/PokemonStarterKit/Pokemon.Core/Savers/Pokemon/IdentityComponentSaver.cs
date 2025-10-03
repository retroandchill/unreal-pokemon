using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Model.Pokemon;
using Riok.Mapperly.Abstractions;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGSaving;

namespace Pokemon.Core.Savers.Pokemon;

/// <summary>
/// Represents a component saver responsible for handling the serialization and persistence of
/// an <c>UIdentityComponent</c> into an <c>FIdentityComponentInfo</c> structure.
/// </summary>
/// <remarks>
/// <para>
/// This class utilizes the Mapperly library for mapping data between the source <c>UIdentityComponent</c>
/// and the target <c>FIdentityComponentInfo</c>. The mapping strategy is defined as requiring all fields
/// to be explicitly mapped.
/// </para>
/// <para>
/// The class is part of the Pokemon.Core.Savers.Pokemon namespace and integrates with Unreal Engine's
/// component saving system. It builds upon functionality from the <c>UComponentSaver</c> base class.
/// </para>
/// </remarks>
/// <seealso cref="UComponentSaver"/>
/// <seealso cref="UIdentityComponent"/>
/// <seealso cref="FIdentityComponentInfo"/>
[UClass]
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
[ComponentSaver<UIdentityComponent, FIdentityComponentInfo>(LoggerClass = typeof(LogPokemonCore))]
public partial class UIdentityComponentSaver : UComponentSaver
{
    private partial FIdentityComponentInfo SaveComponent(UIdentityComponent component);

    [MapperIgnoreTarget(nameof(UIdentityComponent.InitFunction))]
    private partial void LoadComponent(FIdentityComponentInfo info, UIdentityComponent component);
}
