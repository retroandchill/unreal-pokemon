using System.Collections.Immutable;
using GameDataAccessTools.Core.Views;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Core;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Converters;

/// <summary>
/// The BaseStatsConverter is responsible for serializing and deserializing a mapping
/// between Pokémon stats and their corresponding integer values, represented as
/// an IReadOnlyDictionary of FName keys and int values.
/// </summary>
/// <remarks>
/// This class implements the IPbsConverter interface for the <see cref="IReadOnlyDictionary{TKey,TValue}"/> type.
/// It is primarily used to handle conversions of Pokémon base stat data between a CSV
/// string representation and an in-memory dictionary representation.
/// The order of the stats in both serialization and deserialization is determined
/// by their PBS (Pokémon Base Stats) order from the Stats repository.
/// </remarks>
/// <seealso cref="IPbsConverter{T}" />
/// <seealso cref="IReadOnlyDictionary{FName, int}" />
public sealed class BaseStatsConverter : IPbsConverter<IReadOnlyDictionary<FName, int>>
{
    private readonly ImmutableList<FName> _statOrder;

    /// <inheritdoc />
    public Type Type => typeof(IReadOnlyDictionary<FName, int>);

    /// <summary>
    /// Converts base statistics from a structured game data representation into a PBS-format compatible representation
    /// and vice versa. This class focuses on manipulating the main stats of a Pokémon, ordered according to their
    /// predefined PBS order.
    /// </summary>
    /// <remarks>
    /// <para>
    /// The <see cref="BaseStatsConverter"/> leverages the <see cref="GameData.Stats"/> repository to determine the main
    /// statistics and their order within the Pokémon game data. This ensures that only the relevant stats are included,
    /// and they are processed in the correct sequence when serializing to or deserializing from PBS format.
    /// </para>
    /// <para>
    /// This converter operates on objects implementing <see cref="IReadOnlyDictionary{TKey, TValue}"/> where keys represent
    /// the stat names (of type <see cref="FName"/>) and values are the corresponding stat values (of type <see cref="int"/>).
    /// </para>
    /// </remarks>
    public BaseStatsConverter()
    {
        var statsRepository = GameData.Stats;
        ArgumentNullException.ThrowIfNull(statsRepository);
        _statOrder = statsRepository
            .Entries.AsValueEnumerable(s => (s.ID, s.IsMainStat, s.PBSOrder))
            .Where(x => x.IsMainStat)
            .OrderBy(x => x.PBSOrder)
            .Select(x => x.ID)
            .ToImmutableList();
    }

    string IPbsConverter.WriteCsvValue(object? value, PbsScalarDescriptor schema, string? sectionName)
    {
        return WriteCsvValue((IReadOnlyDictionary<FName, int>)value!, schema, sectionName);
    }

    /// <inheritdoc />
    public string WriteCsvValue(IReadOnlyDictionary<FName, int> value, PbsScalarDescriptor schema, string? sectionName)
    {
        return ExportStatsValues(value);
    }

    private string ExportStatsValues(IReadOnlyDictionary<FName, int> value)
    {
        return string.Join(",", _statOrder.Select(x => value.GetValueOrDefault(x, 1)));
    }

    object IPbsConverter.GetCsvValue(string input, PbsScalarDescriptor scalarDescriptor, string? sectionName)
    {
        return GetCsvValue(input, scalarDescriptor, sectionName);
    }

    /// <inheritdoc />
    public IReadOnlyDictionary<FName, int> GetCsvValue(
        string input,
        PbsScalarDescriptor scalarDescriptor,
        string? sectionName
    )
    {
        var values = input.Split(',').Select(int.Parse).ToImmutableList();
        if (values.Count != _statOrder.Count)
        {
            throw new ArgumentException("Invalid number of values");
        }

        foreach (var value in values)
        {
            ArgumentOutOfRangeException.ThrowIfLessThan(value, 0, nameof(value));
        }

        return _statOrder.Zip(values, (x, y) => (Stat: x, Value: y)).ToImmutableDictionary(x => x.Stat, x => x.Value);
    }
}
