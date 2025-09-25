namespace RPG.Battle.Traits;

public interface ITrait
{
    
}

public interface ITrait<in TContext> : ITrait
{
    ValueTask Trigger(TContext context, CancellationToken cancellationToken = default);
}