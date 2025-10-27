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

/// <summary>
/// UBattlerSubsystem is responsible for managing services related to battle statistics
/// and battle abilities in the Pokémon battle system. It provides access to key
/// services required for battler components to perform their operations, such as
/// statistics calculations and ability management.
/// </summary>
/// <remarks>
/// This class extends UCSGameInstanceSubsystem and initializes required services, including:
/// - IBattleStatsService for handling battle statistics calculations.
/// - IBattleAbilityService for managing abilities associated with battlers.
/// The subsystem is initialized within the game instance's subsystem collection and acts
/// as a core dependency provider for components relying on these battle-specific services.
/// </remarks>
[UClass]
public partial class UBattlerSubsystem : UCSGameInstanceSubsystem
{
    /// <summary>
    /// Provides access to battle statistics within the subsystem. Responsible for managing
    /// and facilitating operations involving battle stats during combat scenarios.
    /// </summary>
    public IBattleStatsService BattleStatsService { get; private set; } = null!;

    /// <summary>
    /// Manages and facilitates operations involving abilities within the battle system.
    /// Provides services to handle activation, deactivation, and management of abilities
    /// associated with battlers during combat scenarios.
    /// </summary>
    public IBattleAbilityService BattleAbilityService { get; private set; } = null!;

    /// <inheritdoc />
    protected override void Initialize_Implementation(FSubsystemCollectionBaseRef collection)
    {
        var serviceProvider = collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        BattleStatsService = serviceProvider.GetRequiredService<IBattleStatsService>();
        BattleAbilityService = serviceProvider.GetRequiredService<IBattleAbilityService>();
    }
}
