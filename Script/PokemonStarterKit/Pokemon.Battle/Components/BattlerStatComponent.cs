using Pokemon.Battle.Entities;
using Pokemon.Battle.Subsystems;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerStatComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UBattlerSubsystem BattlerSubsystem { get; set; }

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

    [UProperty]
    private TMap<FBattleStatHandle, int> StatStages { get; }

    public int EffectiveSpeed =>
        BattlerSubsystem.BattleStatsService.GetEffectiveSpeed((UBattler)OwningEntity);

    public bool IsFainted => HP == 0;

    protected override void PreInitialize()
    {
        BattlerSubsystem = GetGameInstanceSubsystem<UBattlerSubsystem>();
    }

    public int GetStatStages(FBattleStatHandle handle)
    {
        return StatStages.GetValueOrDefault(handle);
    }

    public bool StatStageAtMax(FBattleStatHandle handle)
    {
        return GetStatStages(handle) >= BattlerSubsystem.BattleStatsService.StatStageMaximum;
    }
}
