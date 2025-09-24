using RPG.Battle.Model;

namespace RPG.Battle.Services;

public interface IBattleOutcomeService
{
    ValueTask<FBattleOutcome?> CheckOutcomeAsync(
        FTurnContext context,
        CancellationToken cancellationToken = default
    );
}
