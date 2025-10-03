using RPG.Battle.Actions;
using RPG.Battle.Model;

namespace RPG.Battle.Services;

/// <summary>
/// Defines the contract for a service responsible for determining the turn order
/// in a battle scenario.
/// </summary>
public interface ITurnOrderService
{
    /// <summary>
    /// Asynchronously determines the turn order for a battle scenario by yielding a sequence of actions.
    /// </summary>
    /// <param name="context">The context of the current turn, including battle state and turn number.</param>
    /// <param name="cancellationToken">A token to monitor for cancellation requests.</param>
    /// <returns>An asynchronous enumerable of actions representing the turn order.</returns>
    IAsyncEnumerable<IAction> GetTurnOrderAsync(FTurnContext context, CancellationToken cancellationToken = default);
}
