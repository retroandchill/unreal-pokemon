using Pokemon.Battle.Entities;
using Pokemon.Data.Model.PBS;

namespace Pokemon.Battle.Services.Abilities;

/// <summary>
/// Provides services and utilities related to battle abilities for battlers in the Pokémon battle system.
/// </summary>
public interface IBattleAbilityService
{
    /// <summary>
    /// Determines whether a battler has an active ability in the current battle context.
    /// </summary>
    /// <param name="battler">The battler whose abilities are being checked.</param>
    /// <param name="ignoreFained">Specifies whether to ignore if the battler is fainted while checking abilities.</param>
    /// <param name="checkAbility">The specific ability to check for. If not specified, checks for any active ability.</param>
    /// <returns>True if the battler has the specified active ability, or any active ability if none specified; otherwise, false.</returns>
    bool HasActiveAbility(UBattler battler, bool ignoreFained = false, FAbilityHandle checkAbility = default);
}
