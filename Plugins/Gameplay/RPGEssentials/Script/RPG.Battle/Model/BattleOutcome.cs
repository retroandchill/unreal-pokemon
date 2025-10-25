using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;

namespace RPG.Battle.Model;

/// <summary>
/// Represents possible outcomes of a battle in the RPG system.
/// </summary>
[UEnum]
public enum EBattleResult : byte
{
    /// <summary>
    /// The battle was a win.
    /// </summary>
    Win = 0,

    /// <summary>
    /// The battle was a loss.
    /// </summary>
    Lose = 1,

    /// <summary>
    /// The battle was a draw.
    /// </summary>
    Draw = 2,

    /// <summary>
    /// The battle was aborted.
    /// </summary>
    Aborted = 3,

    /// <summary>
    /// The battle ended for a custom reason.
    /// </summary>
    Custom = 4,
}

/// <summary>
/// Represents the outcome of a battle and provides details about the result and additional data.
/// </summary>
/// <param name="Result">The result of the battle.</param>
/// <remarks>
/// This struct is a part of the RPG battle model and is used to encapsulate the results of a battle.
/// The result indicates whether the battle was won, lost, drawn, aborted, or had a custom outcome.
/// Additional data can be included to provide context or auxiliary information about the outcome.
/// </remarks>
[UStruct]
public readonly partial record struct FBattleOutcome(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] EBattleResult Result
)
{
    /// <summary>
    /// Represents additional context or auxiliary details related to the outcome of a battle.
    /// </summary>
    /// <remarks>
    /// This property serves as a container for extended data that may vary depending on the specific requirements
    /// of the battle result. It is designed to support flexible and customizable scenarios by holding optional
    /// supplemental information.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public FInstancedStruct AdditionalData { get; init; }
}
