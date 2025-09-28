using System.Collections.Immutable;
using System.Runtime.CompilerServices;
using Pokemon.Battle.Actions;
using Pokemon.Battle.Actors;
using Pokemon.Battle.Entities;
using Pokemon.Battle.Services.Events.SpeedCalc;
using RPG.Battle.Actions;
using RPG.Battle.Model;
using RPG.Battle.Services;
using UnrealInject.SourceGenerator.Attributes;

namespace Pokemon.Battle.Services;

[Service]
public class PokemonTurnOrderService : ITurnOrderService
{
    public async IAsyncEnumerable<IAction> GetTurnOrderAsync(
        FTurnContext context,
        [EnumeratorCancellation] CancellationToken cancellationToken = default
    )
    {
        if (context.BattleContextActor is not ABattleContext battleContext)
        {
            throw new ArgumentException("Invalid battle context actor");
        }

        var selectedActions = new List<IPokemonAction>();
        await foreach (var action in GetActionsAsync(battleContext.AllBattlers, cancellationToken))
        {
            selectedActions.Add(action);
        }

        foreach (
            var action in selectedActions
                .OrderByDescending(a => a.Priority)
                .ThenByDescending(a => a.User.EffectiveSpeed)
        )
        {
            yield return action;
        }
    }

    private IAsyncEnumerable<IPokemonAction> GetActionsAsync(
        IEnumerable<UBattler> battlers,
        CancellationToken cancellationToken = default
    )
    {
        throw new NotImplementedException();
    }
}
