using Pokemon.Battle.Actions;
using Pokemon.Battle.Actors;
using Pokemon.Battle.Entities;
using Pokemon.Core.Entities;
using RPG.Battle.Actions;
using RPG.Battle.Model;
using RPG.Battle.Services;

namespace Pokemon.Battle.Services;

public class PokemonTurnOrderService : ITurnOrderService
{
    public async IAsyncEnumerable<IAction> GetTurnOrderAsync(
        FTurnContext context,
        CancellationToken cancellationToken = default
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
                .OrderByDescending(CalculateEffectivePriority)
                .ThenByDescending(a => CalculateEffectiveSpeed(a.User))
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

    private int CalculateEffectivePriority(IPokemonAction action)
    {
        // TODO: We need to account for the effects of abilities, hold items, etc.
        return action.Priority;
    }

    private int CalculateEffectiveSpeed(UPokemon pokemon)
    {
        // TODO: We need to account for the effects of abilities, hold items, etc.
        return pokemon.Speed;
    }
}
