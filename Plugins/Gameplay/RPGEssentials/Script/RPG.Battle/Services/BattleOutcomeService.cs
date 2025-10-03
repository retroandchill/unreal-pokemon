using RPG.Battle.Model;

namespace RPG.Battle.Services;

/// <summary>
/// Represents a service that determines the outcome of a battle.
/// This service provides functionality for evaluating the battle's result
/// based on the current turn context and other parameters.
/// </summary>
public interface IBattleOutcomeService
{
    /// <summary>
    /// Asynchronously checks the outcome of a battle based on the provided turn context.
    /// This method evaluates the current state of the battle to determine whether it has concluded
    /// and returns the result if the outcome has been decided.
    /// </summary>
    /// <param name="context">
    /// The context of the current turn, including information about the battle and the turn number.
    /// </param>
    /// <param name="cancellationToken">
    /// A token to monitor for cancellation requests.
    /// </param>
    /// <return>
    /// A task that represents the asynchronous operation. Returns an <see cref="FBattleOutcome"/> if the outcome
    /// has been determined, or null if the battle continues.
    /// </return>
    ValueTask<FBattleOutcome?> CheckOutcomeAsync(FTurnContext context, CancellationToken cancellationToken = default);
}
