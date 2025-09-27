using Pokemon.Data.Model.HardCoded;

namespace Pokemon.Core.Services.Stats;

public interface IStatCalculationService
{
    int CalculateStat(
        FMainStatHandle stat,
        int baseValue,
        int level,
        int iv,
        int ev,
        int natureModifer
    );
}
