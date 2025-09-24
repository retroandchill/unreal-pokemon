namespace RPG.Battle.Actions;

public interface IAction
{
    ValueTask ExecuteAsync(CancellationToken cancellationToken = default);
}
