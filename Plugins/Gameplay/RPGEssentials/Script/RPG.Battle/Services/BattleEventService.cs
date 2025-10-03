using RPG.Battle.Model;

namespace RPG.Battle.Services;

/// <summary>
/// Interface for handling battle events during a turn-based combat system.
/// Provides lifecycle hooks for the start and end of a turn.
/// </summary>
public interface IBattleEventService
{
    /// <summary>
    /// Called at the start of a turn to handle any event logic that must occur before the turn begins.
    /// This method provides an opportunity to execute pre-turn operations such as status effects,
    /// buffs, or any mechanics related to the turn's initialization.
    /// </summary>
    /// <param name="context">The context of the current turn, containing state information about the battle and turn number.</param>
    /// <param name="cancellationToken">A token to cancel the asynchronous operation, used to handle task cancellation effectively.</param>
    /// <returns>A task that represents the asynchronous operation performed during the start of the turn.</returns>
    ValueTask OnTurnStartAsync(FTurnContext context, CancellationToken cancellationToken = default);

    /// <summary>
    /// Called at the end of a turn to handle event logic that must occur after the turn concludes.
    /// This method provides an opportunity to execute post-turn operations such as state cleanup,
    /// applying lingering effects, or finalizing turn-specific data.
    /// </summary>
    /// <param name="context">The context of the current turn, containing state information about the battle and turn number.</param>
    /// <param name="cancellationToken">A token to cancel the asynchronous operation, used to handle task cancellation effectively.</param>
    /// <returns>A task that represents the asynchronous operation performed at the end of the turn.</returns>
    ValueTask OnTurnEndAsync(FTurnContext context, CancellationToken cancellationToken = default);
}
