using System.Collections.Immutable;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;
using UnrealSharp;

namespace Pokemon.Editor.Serialization.Mappers;

[Mapper(
    RequiredMappingStrategy = RequiredMappingStrategy.Target,
    PreferParameterlessConstructors = false
)]
public static partial class SpeciesMapper
{
    private const int EvolutionLearnLevel = 0;

    [MapProperty(
        nameof(SpeciesInfo.EVYield),
        nameof(FSpecies.EVYield),
        Use = nameof(MapEvYieldList)
    )]
    public static partial FSpecies ToSpecies(this SpeciesInfo speciesInfo);

    [MapProperty(
        nameof(SpeciesInfo.EVYield),
        nameof(FSpecies.EVYield),
        Use = nameof(MapEvYieldDictionary)
    )]
    public static partial SpeciesInfo ToSpeciesInfo(this FSpecies species);

    private static LevelUpMoveInfo ToLevelUpMoveInfo(this FLevelUpMove move)
    {
        return move.Match(
            level => new LevelUpMoveInfo(level, move.Move),
            () => new LevelUpMoveInfo(EvolutionLearnLevel, move.Move)
        );
    }

    private static FLevelUpMove ToLevelUpMove(this LevelUpMoveInfo move)
    {
        return move.Level == EvolutionLearnLevel
            ? FLevelUpMove.LevelUp(move.Level, move.Move)
            : FLevelUpMove.Evolution(move.Move);
    }

    private static IReadOnlyDictionary<FMainStatHandle, int> MapEvYieldList(
        IReadOnlyList<EvYield> evYields
    )
    {
        return evYields.ToDictionary(x => new FMainStatHandle(x.Stat), x => x.Amount);
    }

    private static IReadOnlyList<EvYield> MapEvYieldDictionary(
        IReadOnlyDictionary<FMainStatHandle, int> evYields
    )
    {
        return evYields.Select(x => new EvYield(x.Key, x.Value)).ToImmutableList();
    }
}
