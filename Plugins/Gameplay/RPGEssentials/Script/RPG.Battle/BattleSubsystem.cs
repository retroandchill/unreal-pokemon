using Microsoft.Extensions.DependencyInjection;
using RPG.Battle.Model;
using RPG.Battle.Services;
using UnrealInject.Subsystems;
using UnrealSharp.Engine;
using UnrealSharp.LevelEditor;
using UnrealSharp.UnrealSharpCore;

namespace RPG.Battle;

public class UBattleSubsystem : UCSWorldSubsystem
{
    private ITurnOrderService _turnOrderService = null!;
    private IBattleOutcomeService _battleOutcomeService = null!;
    private IBattleEventService _battleEventService = null!;
    private CancellationTokenSource? _cancellationTokenSource;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var serviceProvider =
            collection.InitializeRequiredSubsystem<UDependencyInjectionWorldSubsystem>();
        _turnOrderService = serviceProvider.GetRequiredService<ITurnOrderService>();
        _battleOutcomeService = serviceProvider.GetRequiredService<IBattleOutcomeService>();
        _battleEventService = serviceProvider.GetRequiredService<IBattleEventService>();
    }

#if WITH_EDITOR
    protected override bool ShouldCreateSubsystem()
    {
        var levelEditorSubsystem = GetEditorSubsystem<ULevelEditorSubsystem>();
        return levelEditorSubsystem.IsInPlayInEditor();
    }
#endif

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
