using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;
using UnrealSharp.InteractiveUI;

namespace InteractiveUI.Core;

/// <summary>
/// Provides extension methods for managing widget layers in a user interface.
/// </summary>
[PublicAPI]
public static class WidgetLayerExtensions
{
    /// <summary>
    /// Pushes a widget of the specified type into a specified layer for the given player controller and optionally configures it.
    /// </summary>
    /// <param name="playerController">The player controller to push the content from.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the content should be pushed.</param>
    /// <param name="widgetClass">The class of the widget to be pushed into the layer.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>The created widget of the specified type.</returns>
    public static TWidget PushContentToLayer<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        TSubclassOf<TWidget> widgetClass,
        Action<TWidget>? configureWidget = null
    )
        where TWidget : UCommonActivatableWidget
    {
        var newWidget = playerController
            .GetLocalPlayerFromController()
            .PushContentToLayer(layerName, widgetClass);
        configureWidget?.Invoke(newWidget);
        return newWidget;
    }

    /// <summary>
    /// Pushes a widget of the specified type into a specified layer for the given player controller and optionally configures it.
    /// </summary>
    /// <param name="playerController">The player controller to push the content from.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the content should be pushed.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>The created widget of the specified type.</returns>
    /// <exception cref="InvalidOperationException">When the specified layer does not exist</exception>
    public static TWidget PushContentToLayer<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        Action<TWidget>? configureWidget = null
    )
        where TWidget : UCommonActivatableWidget
    {
        var newWidget = playerController
            .GetLocalPlayerFromController()
            .PushContentToLayer<TWidget>(layerName);
        if (newWidget is null)
        {
            throw new InvalidOperationException(
                $"Failed to push content to layer {layerName}, because it does not exist"
            );
        }
        configureWidget?.Invoke(newWidget);
        return newWidget;
    }

    /// <summary>
    /// Asynchronously pushes a widget of the specified type into a specified layer
    /// for the given player controller, optionally suspends input until the operation completes, and optionally configures it.
    /// </summary>
    /// <param name="playerController">The player controller initiating the content push.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the widget should be pushed.</param>
    /// <param name="widgetClass">The soft class pointer of the widget to be pushed into the layer.</param>
    /// <param name="suspendInputUntilComplete">Specifies whether to suspend the input until the operation is complete.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <param name="cancellationToken">A cancellation token to cancel the asynchronous operation.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>A task representing the asynchronous operation. When completed, provides the created widget of the specified type.</returns>
    /// <exception cref="InvalidOperationException">When the specified layer does not exist</exception>
    /// <exception cref="InvalidCastException">If the specified class is not a valid subclass of the widget</exception>
    public static ValueTask<TWidget> PushContentToLayerAsync<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        TSoftClassPtr<TWidget> widgetClass,
        bool suspendInputUntilComplete = true,
        Action<TWidget>? configureWidget = null,
        CancellationToken cancellationToken = default
    )
        where TWidget : UCommonActivatableWidget
    {
        return UCSPushWidgetToLayerAsync.PushWidgetToLayerAsync(
            playerController,
            layerName,
            widgetClass,
            suspendInputUntilComplete,
            configureWidget,
            cancellationToken
        );
    }

    /// <summary>
    /// Asynchronously pushes a widget of the specified type into a specified layer for the given player controller, with optional configuration and input suspension.
    /// </summary>
    /// <param name="playerController">The player controller to initiate the content push.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the content should be pushed.</param>
    /// <param name="widgetClass">A soft reference to the class of the widget to be pushed into the layer.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <param name="cancellationToken">A token to monitor for cancellation requests, allowing the operation to be cancelled.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>A task that represents the asynchronous operation. The result contains the widget of the specified type when the operation completes.</returns>
    /// <exception cref="InvalidOperationException">When the specified layer does not exist</exception>
    /// <exception cref="InvalidCastException">If the specified class is not a valid subclass of the widget</exception>
    public static ValueTask<TWidget> PushContentToLayerAsync<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        TSoftClassPtr<TWidget> widgetClass,
        Action<TWidget>? configureWidget,
        CancellationToken cancellationToken = default
    )
        where TWidget : UCommonActivatableWidget
    {
        return playerController.PushContentToLayerAsync(
            layerName,
            widgetClass,
            true,
            configureWidget,
            cancellationToken
        );
    }

    /// <summary>
    /// Asynchronously pushes a widget of the specified type into a specified layer for the given player controller and optionally configures it.
    /// </summary>
    /// <param name="playerController">The player controller to push the content from.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the content should be pushed.</param>
    /// <param name="suspendInputUntilComplete">Indicates whether input should be suspended until the operation is completed.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>A task that represents the asynchronous operation. The task result contains the created widget of the specified type.</returns>
    /// <exception cref="InvalidOperationException">When the specified layer does not exist</exception>
    /// <exception cref="InvalidCastException">If the specified class is not a valid subclass of the widget</exception>
    public static ValueTask<TWidget> PushContentToLayerAsync<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        bool suspendInputUntilComplete = true,
        Action<TWidget>? configureWidget = null,
        CancellationToken cancellationToken = default
    )
        where TWidget : UCommonActivatableWidget
    {
        return playerController.PushContentToLayerAsync(
            layerName,
            typeof(TWidget),
            suspendInputUntilComplete,
            configureWidget,
            cancellationToken
        );
    }

    /// <summary>
    /// Asynchronously pushes a widget of the specified type into a specified layer for the given player controller, optionally configuring it and handling input suspension.
    /// </summary>
    /// <param name="playerController">The player controller managing the content push operation.</param>
    /// <param name="layerName">The gameplay tag identifying the layer where the widget should be pushed.</param>
    /// <param name="configureWidget">An optional action to configure the widget after it is created.</param>
    /// <param name="cancellationToken">The cancellation token to observe for operation cancellation.</param>
    /// <typeparam name="TWidget">The type of the activatable widget being pushed into the layer.</typeparam>
    /// <returns>A ValueTask representing the asynchronous operation, with the created widget of the specified type as its result.</returns>
    /// <exception cref="InvalidOperationException">When the specified layer does not exist</exception>
    /// <exception cref="InvalidCastException">If the specified class is not a valid subclass of the widget</exception>
    public static ValueTask<TWidget> PushContentToLayerAsync<TWidget>(
        this APlayerController playerController,
        FGameplayTag layerName,
        Action<TWidget>? configureWidget,
        CancellationToken cancellationToken = default
    )
        where TWidget : UCommonActivatableWidget
    {
        return playerController.PushContentToLayerAsync(
            layerName,
            true,
            configureWidget,
            cancellationToken
        );
    }
}
