using Retro.ReadOnlyParams.Annotations;

namespace RPG.Battle.Traits;

public sealed class SimpleTrait<TContext>([ReadOnly] Action<TContext> action) : ITrait<TContext>
{
    public ValueTask Trigger(TContext context, CancellationToken cancellationToken = default)
    {
        action(context);
        return ValueTask.CompletedTask;
    }
}

public sealed class SimpleAsyncTrait<TContext>([ReadOnly] Func<TContext, CancellationToken, ValueTask> func) : ITrait<TContext>
{
    public ValueTask Trigger(TContext context, CancellationToken cancellationToken = default)
    {
        return func(context, cancellationToken);
    }
}