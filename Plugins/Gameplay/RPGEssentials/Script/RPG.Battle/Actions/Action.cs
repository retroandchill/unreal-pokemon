namespace RPG.Battle.Actions;

/// <summary>
/// Represents a general action to be executed within the battle system.
/// </summary>
public interface IAction
{
    /// <summary>
    /// Executes the defined action asynchronously within the battle system.
    /// </summary>
    /// <param name="cancellationToken">
    /// A token that allows the operation to be canceled before completion.
    /// </param>
    /// <returns>
    /// A task that represents the completion of this action's asynchronous execution.
    /// </returns>
    ValueTask ExecuteAsync(CancellationToken cancellationToken = default);
}
