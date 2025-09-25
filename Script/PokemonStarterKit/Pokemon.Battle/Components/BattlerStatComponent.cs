using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerStatComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int Level { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int HP { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int MaxHP { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Battler|Stats")]
    public int Attack { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Battler|Stats")]
    public int Defense { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int SpecialAttack { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int SpecialDefense { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Stats")]
    public int Speed { get; set; }
}
