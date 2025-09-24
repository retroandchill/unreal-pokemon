using UnrealSharp.Attributes;
using UnrealSharp.Engine;

namespace RPG.Battle.Model;

[UStruct]
public readonly record struct FTurnContext(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] AActor BattleContextActor,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int TurnNumber
);
