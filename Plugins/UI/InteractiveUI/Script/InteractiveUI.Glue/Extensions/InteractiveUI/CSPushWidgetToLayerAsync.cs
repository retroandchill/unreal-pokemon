using InteractiveUI.Glue.Extensions;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpAsync;

namespace UnrealSharp.InteractiveUI;

// ReSharper disable once InconsistentNaming
internal partial class UCSPushWidgetToLayerAsync
{
    private readonly TaskCompletionSource<UCommonActivatableWidget> _tcs = new();

    private readonly Action _onAsyncCompleted;
    private Action<UCommonActivatableWidget>? _configureWidget;

    internal UCSPushWidgetToLayerAsync()
    {
        _onAsyncCompleted = OnAsyncCompleted;
    }

    private void OnAsyncCompleted()
    {
        switch (GetResult(out var widget))
        {
            case EAsyncLoadSuccessState.Success:
                _tcs.SetResult(widget);
                break;
            case EAsyncLoadSuccessState.NoSuchLayer:
                _tcs.SetException(new InvalidOperationException($"No layer named {LayerName}"));
                break;
            case EAsyncLoadSuccessState.Cancelled:
                _tcs.SetCanceled();
                break;
            case EAsyncLoadSuccessState.InProgress:
                break;
            case EAsyncLoadSuccessState.BeforePush:
                _configureWidget?.Invoke(widget);
                break;
            default:
                throw new InvalidOperationException("Unknown result enum");
        }
    }

    public static async ValueTask<T> PushWidgetToLayerAsync<T>(
        APlayerController playerController,
        FGameplayTag layerName,
        TSoftClassPtr<T> widgetClass,
        bool suspendInputUntilComplete = true,
        Action<T>? configureWidget = null,
        CancellationToken cancellationToken = default
    )
        where T : UCommonActivatableWidget
    {
        var loader = NewObject<UCSPushWidgetToLayerAsync>(playerController);
        loader._configureWidget =
            configureWidget != null
                ? widget =>
                {
                    configureWidget((T)widget);
                }
                : null;
        NativeAsyncUtilities.InitializeAsyncAction(loader, loader._onAsyncCompleted);
        loader.PushWidgetToLayerStack(
            playerController,
            layerName,
            suspendInputUntilComplete,
            widgetClass.Cast<UCommonActivatableWidget>()
        );
        cancellationToken.Register(loader.Cancel);

        return (T)await loader._tcs.Task;
    }
}
