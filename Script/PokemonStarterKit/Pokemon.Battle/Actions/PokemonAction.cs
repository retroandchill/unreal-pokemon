using Pokemon.Core.Entities;
using RPG.Battle.Actions;

namespace Pokemon.Battle.Actions;

public interface IPokemonAction : IAction
{
    int Priority { get; }

    UPokemon User { get; }
}
