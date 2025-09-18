using Pokemon.Editor.Serialization.Pbs.Attributes;
using UnrealSharp;
using UnrealSharp.GameplayTags;

namespace Pokemon.Editor.Serialization.Model.Pbs;

public readonly record struct IntBounds(
    [property: PbsRange<int>(0)] int Min,
    [property: PbsRange<int>(1)] int Max
);

public record BerryPlantInfo
{
    [PbsKey]
    public FName ID { get; init; }

    [PbsIndex]
    public int RowIndex { get; init; }

    [PbsRange<int>(1)]
    public int HoursPerStage { get; init; } = 3;

    [PbsRange<int>(0)]
    public int DryingPerHour { get; init; } = 15;

    public IntBounds Yield { get; init; } = new(2, 5);
}
