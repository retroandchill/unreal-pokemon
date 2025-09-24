using RPG.Battle.Actions;
using RPG.Battle.Model;

namespace RPG.Battle.Services;

public interface ITurnOrderService
{
    IAsyncEnumerable<IAction> GetTurnOrderAsync(
        FTurnContext context,
        CancellationToken cancellationToken = default
    );
}
