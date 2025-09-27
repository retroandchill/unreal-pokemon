using Pokemon.Battle.Entities;
using Pokemon.Core.Entities;
using RPG.Battle.Actions;

namespace Pokemon.Battle.Actions;

public interface IPokemonAction : IAction
{
    int Priority { get; }

    UBattler User { get; }
}
