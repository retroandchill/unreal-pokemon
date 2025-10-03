using System.Collections.Immutable;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;
using UnrealSharp;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// A static class responsible for mapping between <c>SpeciesInfo</c> and <c>FSpecies</c> objects.
/// This class provides methods to convert objects from one type to the other, implementing any required transformations.
/// </summary>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class SpeciesMapper
{
    private const int EvolutionLearnLevel = 0;

    /// <summary>
    /// Maps a <c>SpeciesInfo</c> object to an <c>FSpecies</c> object,
    /// converting and transforming data as required for the target structure.
    /// </summary>
    /// <param name="speciesInfo">The <c>SpeciesInfo</c> instance to map into an <c>FSpecies</c> object.</param>
    /// <returns>An <c>FSpecies</c> object created from the provided <c>SpeciesInfo</c> instance.</returns>
    [MapProperty(nameof(SpeciesInfo.EVYield), nameof(FSpecies.EVYield), Use = nameof(MapEvYieldList))]
    public static partial FSpecies ToSpecies(this SpeciesInfo speciesInfo);

    /// <summary>
    /// Maps an <c>FSpecies</c> object to a <c>SpeciesInfo</c> object,
    /// converting and transforming data as required for the target structure.
    /// </summary>
    /// <param name="species">The <c>FSpecies</c> instance to map into a <c>SpeciesInfo</c> object.</param>
    /// <returns>A <c>SpeciesInfo</c> object created from the provided <c>FSpecies</c> instance.</returns>
    [MapProperty(nameof(SpeciesInfo.EVYield), nameof(FSpecies.EVYield), Use = nameof(MapEvYieldDictionary))]
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

    private static IReadOnlyDictionary<FMainStatHandle, int> MapEvYieldList(IReadOnlyList<EvYield> evYields)
    {
        return evYields.ToDictionary(x => new FMainStatHandle(x.Stat), x => x.Amount);
    }

    private static IReadOnlyList<EvYield> MapEvYieldDictionary(IReadOnlyDictionary<FMainStatHandle, int> evYields)
    {
        return evYields.Select(x => new EvYield(x.Key, x.Value)).ToImmutableList();
    }
}
