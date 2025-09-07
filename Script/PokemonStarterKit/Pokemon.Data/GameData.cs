using GameAccessTools.SourceGenerator.Attributes;
using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Data;

[GameDataProvider]
public static partial class GameData
{
    public static partial UGrowthRateRepository GrowthRates { get; }
    public static partial UGenderRatioRepository GenderRatios { get; }
    public static partial UEggGroupRepository EggGroups { get; }
    public static partial UBodyShapeRepository BodyShapes { get; }
    public static partial UBodyColorRepository BodyColors { get; }
    public static partial UHabitatRepository Habitats { get; }
    public static partial UEvolutionRepository Evolutions { get; }
    public static partial UStatRepository Stats { get; }
    public static partial UNatureRepository Natures { get; }
    public static partial UStatusRepository Statuses { get; }
    public static partial UFieldWeatherRepository FieldWeathers { get; }
    public static partial UEncounterTypeRepository EncounterTypes { get; }
    public static partial UEnvironmentRepository Environments { get; }
    public static partial UBattleWeatherRepository BattleWeathers { get; }
    public static partial UBattleTerrainRepository BattleTerrains { get; }
    public static partial UTargetRepository Targets { get; }
}
