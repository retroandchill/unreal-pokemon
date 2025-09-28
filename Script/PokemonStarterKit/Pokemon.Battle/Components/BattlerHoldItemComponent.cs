using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerHoldItemComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|HoldItem")]
    public FItemHandle? HoldItem { get; set; }

    public bool ItemActive => HoldItem != null;
}
