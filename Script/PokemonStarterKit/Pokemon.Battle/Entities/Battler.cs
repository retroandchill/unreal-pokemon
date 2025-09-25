using Pokemon.Battle.Components;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Entities;

[UClass(ClassFlags.Abstract)]
public partial class UBattler : URPGEntity
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerInfoComponent InfoComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerStatComponent StatComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerAbilityComponent AbilityComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerHoldItemComponent HoldItemComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UBattlerStatusEffectComponent StatusEffectComponent { get; private set; }
}
