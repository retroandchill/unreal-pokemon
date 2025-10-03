using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.Settings;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.StatusEffects;

/// <summary>
/// Represents a status effect that causes paralysis in a Pokémon during a battle.
/// </summary>
/// <param name="settings">The developer settings for the battle system.</param>
/// <remarks>
/// ParalysisStatus is responsible for influencing the speed calculation of a Pokémon
/// when it is inflicted with the paralysis condition. This class implements the
/// <see cref="ISpeedCalcEvaluator"/> interface to participate in the speed calculation
/// logic within the battle system. Paralysis typically results in a speed reduction
/// for the affected Pokémon.
/// </remarks>
/// <example>
/// ParalysisStatus objects are used within the battle system to modify the behavior
/// of affected Pokémon during speed calculation phases.
/// </example>
/// <seealso cref="IDeveloperSettings{UPokemonBattleSettings}"/>
/// <seealso cref="UPokemonBattleSettings"/>
/// <seealso cref="ISpeedCalcEvaluator"/>
[Service]
public class ParalysisStatus(IDeveloperSettings<UPokemonBattleSettings> settings) : ISpeedCalcEvaluator
{
    private readonly UPokemonBattleSettings _settings = settings.Settings;

    int ISpeedCalcEvaluator.Priority => SpeedCalcPriorities.Paralysis;

    void ISpeedCalcEvaluator.Evaluate(UBattler battler, ref float multiplier)
    {
        if (
            battler.StatusEffect == FStatus.PARALYSIS
            && _settings.IgnoresParalysisSpeedDrop.Any(a => battler.HasActiveAbility(a))
        )
        {
            multiplier /= 2;
        }
    }
}
