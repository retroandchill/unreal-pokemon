using UnrealSharp.Attributes;
using UnrealSharp.Engine;

namespace RPG.Battle.Model;

/// <summary>
/// Represents the context of a single turn within a battle.
/// This structure carries information regarding the current turn
/// and the associated battle's state.
/// </summary>
/// <param name="BattleContextActor">The actor that represents the battle context.</param>
/// <param name="TurnNumber">The current turn number within the battle.</param>
[UStruct]
public readonly partial record struct FTurnContext(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] AActor BattleContextActor,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int TurnNumber
);
