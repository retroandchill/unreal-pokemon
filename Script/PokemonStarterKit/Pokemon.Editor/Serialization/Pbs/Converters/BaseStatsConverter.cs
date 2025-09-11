using System.Collections.Immutable;
using GameDataAccessTools.Core.Views;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using ZLinq;

namespace Pokemon.Editor.Serialization.Pbs.Converters;

public sealed class BaseStatsConverter : PbsConverterBase<IReadOnlyDictionary<FName, int>>
{
    private readonly ImmutableList<FName> _statOrder;

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

    public override string WriteCsvValue(
        IReadOnlyDictionary<FName, int> value,
        PbsScalarDescriptor schema,
        string? sectionName
    )
    {
        return string.Join(",", _statOrder.Select(x => value.GetValueOrDefault(x, 1)));
    }

    public override IReadOnlyDictionary<FName, int> GetCsvValue(
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

        return _statOrder
            .Zip(values, (x, y) => (Stat: x, Value: y))
            .ToImmutableDictionary(x => x.Stat, x => x.Value);
    }
}
