using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class AbilityMapper
{
    public static partial FAbility ToAbility(this AbilityInfo abilityInfo);
    
    public static partial AbilityInfo ToAbilityInfo(this FAbility ability);
}