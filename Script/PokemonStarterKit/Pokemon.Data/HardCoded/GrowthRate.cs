using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Views;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FGrowthRate : IGameDataEntry
{
    public static int MaxLevel => UObject.GetDefault<UPokemonDataSettings>().MaxLevel;

    [field: UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadOnly,
        Category = "Identification"
    )]
    public required FName ID { get; init; }

    [field: UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadOnly,
        Category = "Identification"
    )]
    public int RowIndex { get; init; }

    [field: UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadOnly,
        Category = "Display"
    )]
    public required FText Name { get; init; }

    [field: UProperty(
        PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadOnly,
        Category = "Display"
    )]
    public required IReadOnlyList<int> ExpValues { get; init; }

    public Func<int, int>? ExpFormula { get; init; }
}

[UClass]
[GameDataRepository<FGrowthRate>]
public partial class UGrowthRateRepository : UStaticGameDataRepository;

public static partial class GrowthRateViewExtensions
{
    public static int MaxLevel => FGrowthRate.MaxLevel;

    extension(StructView<FGrowthRate> self)
    {
        public int GetMinimumExpForLevel(int level)
        {
            ArgumentOutOfRangeException.ThrowIfLessThan(level, 1, nameof(level));
            level = Math.Min(level, MaxLevel);

            if (level < self.ExpValues.Length)
            {
                return self.ExpValues[level - 1];
            }

            return self.ExpFormula?.Invoke(level)
                ?? throw new InvalidOperationException(
                    $"No Exp formula is defined for growth rate {self.Name}"
                );
        }

        public int MaximumExp => self.GetMinimumExpForLevel(MaxLevel);

        public int AddExp(int exp1, int exp2) => Math.Clamp(exp1 + exp2, 0, self.MaximumExp);

        public int GetLevelFromExp(int exp)
        {
            ArgumentOutOfRangeException.ThrowIfLessThan(exp, 0, nameof(exp));
            var max = MaxLevel;
            if (exp >= self.MaximumExp)
            {
                return max;
            }

            for (var i = 1; i <= max; i++)
            {
                if (exp < self.GetMinimumExpForLevel(i))
                {
                    return i - 1;
                }
            }

            return max;
        }
    }
}
