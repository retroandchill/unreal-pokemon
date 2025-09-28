using System.Diagnostics.CodeAnalysis;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Subsystems;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerAbilityComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UBattlerSubsystem BattlerSubsystem { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Ability")]
    public FAbilityHandle? Ability { get; set; }

    [MemberNotNullWhen(true, nameof(Ability))]
    public bool AbilityActive => IsAbilityActive();

    protected override void PreInitialize()
    {
        BattlerSubsystem = GetGameInstanceSubsystem<UBattlerSubsystem>();
    }

    public bool IsAbilityActive(bool ignoreFainted = false, FAbilityHandle checkAbility = default)
    {
        return BattlerSubsystem.BattleAbilityService.HasActiveAbility(
            (UBattler)OwningEntity,
            ignoreFainted,
            checkAbility
        );
    }
}
