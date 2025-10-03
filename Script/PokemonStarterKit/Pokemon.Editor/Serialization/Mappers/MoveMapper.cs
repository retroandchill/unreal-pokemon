using Pokemon.Data.Model.PBS;
using Pokemon.Editor.Serialization.Model.Pbs;
using Riok.Mapperly.Abstractions;

namespace Pokemon.Editor.Serialization.Mappers;

/// <summary>
/// Provides mapping functionality between <see cref="FMove"/> and <see cref="MoveInfo"/> objects.
/// </summary>
[Mapper(RequiredMappingStrategy = RequiredMappingStrategy.Target, PreferParameterlessConstructors = false)]
public static partial class MoveMapper
{
    private const int AlwaysHitsAccuracy = 0;
    private const int NoDamagePower = 0;
    private const int VariableDamagePower = 1;
    private const int GuaranteedEffectChance = 0;

    /// <summary>
    /// Maps a <see cref="MoveInfo"/> object to an <see cref="FMove"/> object.
    /// </summary>
    /// <param name="moveInfo">The source <see cref="MoveInfo"/> to map from.</param>
    /// <returns>A mapped <see cref="FMove"/> object.</returns>
    [MapPropertyFromSource(nameof(FMove.DamageType), Use = nameof(MapDamageType))]
    [MapPropertyFromSource(nameof(FMove.AlwaysHits), Use = nameof(MapAlwaysHits))]
    [MapPropertyFromSource(nameof(FMove.GuaranteedEffect), Use = nameof(MapGuaranteedEffect))]
    public static partial FMove ToMove(this MoveInfo moveInfo);

    /// <summary>
    /// Maps an <see cref="FMove"/> object to a <see cref="MoveInfo"/> object.
    /// </summary>
    /// <param name="move">The source <see cref="FMove"/> to map from.</param>
    /// <returns>A mapped <see cref="MoveInfo"/> object.</returns>
    [MapPropertyFromSource(nameof(MoveInfo.Power), Use = nameof(MapPower))]
    [MapPropertyFromSource(nameof(MoveInfo.Accuracy), Use = nameof(MapAccuracy))]
    [MapPropertyFromSource(nameof(MoveInfo.EffectChance), Use = nameof(MapEffectChance))]
    public static partial MoveInfo ToMoveInfo(this FMove move);

    private static int MapPower(this in FMove move)
    {
        return move.DamageType switch
        {
            EDamageType.NoDamage => NoDamagePower,
            EDamageType.FixedPower => move.Power,
            EDamageType.VariablePower => VariableDamagePower,
            _ => throw new InvalidOperationException(),
        };
    }

    private static int MapAccuracy(this in FMove move)
    {
        return move.AlwaysHits ? AlwaysHitsAccuracy : move.Accuracy;
    }

    private static int MapEffectChance(this in FMove move)
    {
        return move.GuaranteedEffect ? GuaranteedEffectChance : move.EffectChance;
    }

    private static EDamageType MapDamageType(this MoveInfo moveInfo)
    {
        return moveInfo.Power switch
        {
            NoDamagePower => EDamageType.NoDamage,
            VariableDamagePower => EDamageType.VariablePower,
            _ => EDamageType.FixedPower,
        };
    }

    private static bool MapAlwaysHits(this MoveInfo moveInfo)
    {
        return moveInfo.Accuracy == AlwaysHitsAccuracy;
    }

    private static bool MapGuaranteedEffect(this MoveInfo moveInfo)
    {
        return moveInfo.EffectChance == GuaranteedEffectChance;
    }
}
