using Pokemon.Data.Model.HardCoded;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components.Battler;

/// <summary>
/// Represents a component responsible for handling status effects applied to a battler entity during a battle.
/// </summary>
/// <remarks>
/// This component is designed to manage and facilitate interaction with the status effects that are applied to battler entities.
/// It provides the necessary framework to retrieve or set the status effect associated with the owning entity.
/// </remarks>
[UClass]
public partial class UBattlerStatusEffectComponent : URPGComponent
{
    /// <summary>
    /// Gets or sets the status effect associated with the battler entity.
    /// </summary>
    /// <remarks>
    /// Represents the current status effect applied to the battler entity in battle scenarios.
    /// This property is a part of the UBattlerStatusEffectComponent and is used to manage the effects that influence the battler's state.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|StatusEffect")]
    public partial FStatusHandle? StatusEffect { get; set; }
}
