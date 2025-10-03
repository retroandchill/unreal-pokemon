using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping functionality between <see cref="AbilityInfo"/> and <see cref="FAbility"/> objects.
/// </summary>
/// <remarks>
/// This static class utilizes source-generated mapping methods from the Mapperly library to convert
/// data models between the editor serialization layer and the game data layer.
/// </remarks>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class AbilityMapper
{
    /// <summary>
    /// Maps an <see cref="AbilityInfo"/> object to an <see cref="FAbility"/> object.
    /// </summary>
    /// <param name="abilityInfo">The source <see cref="AbilityInfo"/> instance to be converted.</param>
    /// <returns>An <see cref="FAbility"/> object that corresponds to the provided <see cref="AbilityInfo"/>.</returns>
    public static partial FAbility ToAbility(this AbilityInfo abilityInfo);

    /// <summary>
    /// Maps an <see cref="FAbility"/> object to an <see cref="AbilityInfo"/> object.
    /// </summary>
    /// <param name="ability">The source <see cref="FAbility"/> instance to be converted.</param>
    /// <returns>An <see cref="AbilityInfo"/> object that corresponds to the provided <see cref="FAbility"/>.</returns>
    public static partial AbilityInfo ToAbilityInfo(this FAbility ability);
}
