using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace RPG.Battle.Model;

[UEnum]
public enum EBattleResult : byte
{
    Win = 0,
    Lose = 1,
    Draw = 2,
    Aborted = 3,
    Custom = 4,
}

[UStruct]
public readonly partial record struct FBattleOutcome(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] EBattleResult Result
)
{
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public FInstancedStruct AdditionalData { get; init; }
}
