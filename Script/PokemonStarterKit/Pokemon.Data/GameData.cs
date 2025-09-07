using GameAccessTools.SourceGenerator.Attributes;
using Pokemon.Data.HardCoded;

namespace Pokemon.Data;

[GameDataProvider]
public static partial class GameData
{
    public static partial UGrowthRateRepository GrowthRates { get; }
}
