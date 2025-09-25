using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerStatusEffectComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|StatusEffect")]
    public FStatusHandle? StatusEffect { get; set; }
}
