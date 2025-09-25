using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerAbilityComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Ability")]
    public FAbilityHandle? Ability { get; set; }
}
