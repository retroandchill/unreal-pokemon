using System.Collections.Immutable;
using Microsoft.Extensions.DependencyInjection;
using Pokemon.Battle.Components;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Abilities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using Pokemon.Battle.Services.Stats;
using Pokemon.Data.Model.PBS;
using RPG.Battle;
using UnrealInject.Subsystems;
using UnrealSharp.Attributes;
using UnrealSharp.UnrealSharpCore;
#if WITH_EDITOR
using UnrealSharp.LevelEditor;
#endif


namespace Pokemon.Battle.Subsystems;

[UClass]
public class UBattlerSubsystem : UCSGameInstanceSubsystem
{
    public IBattleStatsService BattleStatsService { get; private set; } = null!;
    public IBattleAbilityService BattleAbilityService { get; private set; } = null!;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var serviceProvider =
            collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        BattleStatsService = serviceProvider.GetRequiredService<IBattleStatsService>();
        BattleAbilityService = serviceProvider.GetRequiredService<IBattleAbilityService>();
    }
}
