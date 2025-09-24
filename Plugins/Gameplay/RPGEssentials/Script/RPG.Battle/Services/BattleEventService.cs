using RPG.Battle.Model;

namespace RPG.Battle.Services;

public interface IBattleEventService
{
    ValueTask OnTurnStartAsync(FTurnContext context, CancellationToken cancellationToken = default);

    ValueTask OnTurnEndAsync(FTurnContext context, CancellationToken cancellationToken = default);
}
