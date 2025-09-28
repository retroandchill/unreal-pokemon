using Microsoft.Extensions.DependencyInjection;
using RPG.Battle.Model;
using RPG.Battle.Services;
using UnrealInject;
using UnrealInject.Subsystems;
using UnrealSharp.Engine;
using UnrealSharp.UnrealSharpCore;
using ZLinq;


namespace RPG.Battle;

public class UBattleSubsystem : UCSGameInstanceSubsystem
{
    private ITurnOrderService _turnOrderService = null!;
    private IBattleOutcomeService _battleOutcomeService = null!;
    private IBattleEventService _battleEventService = null!;
    private CancellationTokenSource? _cancellationTokenSource;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var serviceProvider =
            collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        _turnOrderService = serviceProvider.GetRequiredService<ITurnOrderService>();
        _battleOutcomeService = serviceProvider.GetRequiredService<IBattleOutcomeService>();
        _battleEventService = serviceProvider.GetRequiredService<IBattleEventService>();
    }

    protected override bool ShouldCreateSubsystem()
    {
        return HasRegisteredServices(
            typeof(ITurnOrderService),
            typeof(IBattleOutcomeService),
            typeof(IBattleEventService)
        );
    }

    private static bool HasRegisteredServices(params ReadOnlySpan<Type> serviceTypes)
    {
        return serviceTypes
            .AsValueEnumerable()
            .All(t => FUnrealInjectModule.Instance.Services.Any(s => s.ServiceType == t));
    }

    public async ValueTask<FBattleOutcome> StartBattleAsync(
        AActor battleContextActor,
        CancellationToken externalCancel = default
    )
    {
        if (_cancellationTokenSource is not null)
        {
            await _cancellationTokenSource.CancelAsync();
        }

        _cancellationTokenSource = CancellationTokenSource.CreateLinkedTokenSource(externalCancel);
        var cancellationToken = _cancellationTokenSource.Token;

        var turnNumber = 0;
        FBattleOutcome? outcome = null;

        while (!cancellationToken.IsCancellationRequested)
        {
            turnNumber++;
            var context = new FTurnContext(battleContextActor, turnNumber);

            await _battleEventService.OnTurnStartAsync(context, cancellationToken);

            outcome = await _battleOutcomeService.CheckOutcomeAsync(context, cancellationToken);
            if (cancellationToken.IsCancellationRequested || outcome.HasValue)
                break;

            await foreach (
                var action in _turnOrderService.GetTurnOrderAsync(context, cancellationToken)
            )
            {
                if (cancellationToken.IsCancellationRequested)
                    break;

                await action.ExecuteAsync(cancellationToken);

                outcome = await _battleOutcomeService.CheckOutcomeAsync(context, cancellationToken);
                if (cancellationToken.IsCancellationRequested || outcome.HasValue)
                    break;
            }

            if (cancellationToken.IsCancellationRequested || outcome.HasValue)
                break;

            outcome = await _battleOutcomeService.CheckOutcomeAsync(context, cancellationToken);
            if (cancellationToken.IsCancellationRequested || outcome.HasValue)
                break;

            await _battleEventService.OnTurnEndAsync(context, cancellationToken);

            outcome = await _battleOutcomeService.CheckOutcomeAsync(context, cancellationToken);
            if (cancellationToken.IsCancellationRequested || outcome.HasValue)
                break;
        }

        return outcome ?? new FBattleOutcome(EBattleResult.Aborted);
    }
}
