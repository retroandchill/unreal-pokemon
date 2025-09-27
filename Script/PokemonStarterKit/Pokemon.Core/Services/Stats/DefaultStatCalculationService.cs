using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Core.Services.Stats;

public class DefaultStatCalculationService : IStatCalculationService
{
    public int CalculateStat(
        FMainStatHandle stat,
        int baseValue,
        int level,
        int iv,
        int ev,
        int natureModifer
    )
    {
        // Special case for Shedinja
        if (stat.ID == FStat.HP && baseValue == 1)
            return 1;

        if (stat.Entry.StatType == EStatType.Main)
        {
            return (2 * baseValue + iv + ev / 4) * level / 100 + level + 10;
        }

        return ((2 * baseValue + iv + ev / 4) * level / 100 + 5) * natureModifer / 100;
    }
}
