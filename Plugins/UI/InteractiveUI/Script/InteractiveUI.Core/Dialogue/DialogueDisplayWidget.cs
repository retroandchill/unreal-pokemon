using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.EnhancedInput;
using UnrealSharp.InteractiveUI;
using UnrealSharp.SlateCore;
using UnrealSharp.UMG;
using UnrealSharp.UnrealSharpCommonUI;

namespace InteractiveUI.Core.Dialogue;

/// <summary>
/// A widget designed to display dialogue in a UI context, extending from the base class UCommonActivatableWidget.
/// </summary>
/// <remarks>
/// This class provides functionality for displaying dialogue text, handling player input for advancing dialogue,
/// and managing dialogue-related UI actions. It supports asynchronous operations to allow integration of
/// non-blocking tasks when displaying or interacting with dialogue.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public class UDialogueDisplayWidget : UCommonActivatableWidget
{
    /// <summary>
    /// Represents the `UDialogueBox` property of the `UDialogueDisplayWidget` class, which serves as the primary interface
    /// for displaying and managing dialogue lines within the widget.
    /// </summary>
    /// <remarks>
    /// The `DialogueBox` property is a bound widget in the UI framework, designed to handle dialogue text rendering and interactions.
    /// It supports features such as playing dialogue lines asynchronously and checking the completion state of the current dialogue.
    /// </remarks>
    [UProperty]
    [BindWidget]
    private UDialogueBox DialogueBox { get; }

    /// <summary>
    /// Represents the `PauseIndicator` property of the `UDialogueDisplayWidget` class, used to indicate when dialogue
    /// playback is paused.
    /// </summary>
    /// <remarks>
    /// The `PauseIndicator` property is an optional, bindable UI widget that manages visibility status based on the
    /// dialogue system's active state. It ensures that the user gains visual feedback when the dialogue progression
    /// is temporarily halted.
    /// </remarks>
    [UProperty]
    [BindWidgetOptional]
    private UWidget? PauseIndicator { get; }

    /// <summary>
    /// Represents the `ScrollBox` property of the `UDialogueDisplayWidget` class, which serves as a container for displaying dialogue content with scrollable functionality.
    /// </summary>
    /// <remarks>
    /// The `ScrollBox` property is a UI-bound widget utilized to manage and render dialogue text that may exceed the visible area of the dialogue window.
    /// It provides functionality for smooth scrolling, updating the scroll offset dynamically, and ensuring the visibility of newly added dialogue content.
    /// </remarks>
    [UProperty]
    [BindWidget]
    private UScrollBox ScrollBox { get; }

    /// <summary>
    /// Represents the `AdvanceAction` property of the `UDialogueDisplayWidget` class, which specifies the primary input action
    /// used for advancing dialogue or triggering dialogue-related events within the widget.
    /// </summary>
    /// <remarks>
    /// The `AdvanceAction` property is an input action designed to handle player interactions for progressing through dialogue content.
    /// It is utilized internally to bind input to the widget's advance functionality and supports customizable action mappings.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Actions")]
    private UInputAction AdvanceAction { get; }

    private FUIActionBindingHandle _advanceActionBinding;

    /// <summary>
    /// Specifies the speed at which the content scrolls within the `UDialogueDisplayWidget`.
    /// </summary>
    /// <remarks>
    /// The `ScrollSpeed` property determines the timing for smooth scrolling behavior in the widget.
    /// It is used to calculate the interpolated scrolling offset when advancing dialogue lines.
    /// A smaller value results in slower scrolling, while a larger value increases the scrolling speed.
    /// </remarks>
    [UProperty(PropertyFlags.EditAnywhere, Category = "Display")]
    [UIMin("0.001f")]
    [ClampMin("0.001f")]
    private float ScrollSpeed { get; } = 0.1f;

    private readonly record struct ScrollStatus(float OriginalScroll, float Timer);

    private ScrollStatus? _scroll;

    /// <summary>
    /// Represents an event triggered to advance the dialogue flow within the `UDialogueDisplayWidget` class.
    /// </summary>
    /// <remarks>
    /// The `OnAdvance` event is raised when user input or a specific action is detected to notify that
    /// the current dialogue line should progress. It is primarily used to handle asynchronous dialogue
    /// playback and ensure proper continuation when waiting for user interactions.
    /// </remarks>
    public event Action? OnAdvance;

    private bool AwaitingInput
    {
        get;
        set
        {
            field = value;
            _advanceActionBinding.DisplayInActionBar = value;

            if (value)
            {
                ActivateWidget();
            }
            else
            {
                DeactivateWidget();
            }

            PauseIndicator?.Visibility = value ? ESlateVisibility.SelfHitTestInvisible : ESlateVisibility.Collapsed;
        }
    }

    /// <inheritdoc />
    public override void Construct()
    {
        _advanceActionBinding = RegisterUIActionBinding(new FCSBindUIActionArgs(AdvanceAction, false, OnAdvanceAction));
    }

    /// <inheritdoc />
    public override void Tick(FGeometry myGeometry, float deltaTime)
    {
        base.Tick(myGeometry, deltaTime);

        var bottomScroll = ScrollBox.ScrollOffsetOfEnd;
        if (_scroll.HasValue)
        {
            _scroll = _scroll.Value with { Timer = _scroll.Value.Timer + deltaTime };
            ScrollBox.ScrollOffset = float.Lerp(
                _scroll.Value.OriginalScroll,
                bottomScroll,
                _scroll.Value.Timer / ScrollSpeed
            );

            if (MathLibrary.NearlyEqual_FloatFloat(ScrollBox.ScrollOffset, bottomScroll))
            {
                _scroll = null;
            }
        }
        else if (!MathLibrary.NearlyEqual_FloatFloat(ScrollBox.ScrollOffset, bottomScroll))
        {
            _scroll = new ScrollStatus(ScrollBox.ScrollOffset, 0);
        }
    }

    /// <inheritdoc />
    public override void Destruct()
    {
        RemoveActionBinding(_advanceActionBinding);
    }

    /// <summary>
    /// Displays a dialogue asynchronously using the associated dialogue box.
    /// </summary>
    /// <param name="text">The text content of the dialogue to display.</param>
    /// <param name="cancellationToken">A token to monitor for cancellation requests.</param>
    /// <returns>A Task that represents the asynchronous operation of displaying the dialogue.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Display")]
    public async Task DisplayDialogue(FText text, CancellationToken cancellationToken = default)
    {
        await DialogueBox.PlayLineAsync(text, cancellationToken).ConfigureWithUnrealContext();

        if (text.Empty)
            return;

        var tcs = new TaskCompletionSource();

        OnAdvance += Advance;

        AwaitingInput = true;
        await tcs.Task.ConfigureWithUnrealContext();
        return;

        void Advance()
        {
            OnAdvance -= Advance;
            tcs.SetResult();
        }
    }

    /// <summary>
    /// Displays a dialogue with provided text and waits for the user to make a selection. The selection is handled by the provided callback function.
    /// </summary>
    /// <typeparam name="T">The type of the result returned by the selection callback.</typeparam>
    /// <param name="text">The dialogue text to display. Cannot be empty.</param>
    /// <param name="onChoice">A callback function that handles the selection process and returns a result of type <typeparamref name="T"/>.</param>
    /// <param name="cancellationToken">A token to observe while waiting for the operation to complete, allowing it to be canceled.</param>
    /// <returns>A task that represents the asynchronous operation. The result of the task is the value returned by the <paramref name="onChoice"/> callback.</returns>
    /// <exception cref="InvalidOperationException">Thrown if an attempt is made to display empty text.</exception>
    public async Task<T> DisplayDialogueWithSelection<T>(
        FText text,
        Func<CancellationToken, Task<T>> onChoice,
        CancellationToken cancellationToken = default
    )
    {
        await DialogueBox.PlayLineAsync(text, cancellationToken).ConfigureWithUnrealContext();

        if (text.Empty)
        {
            throw new InvalidOperationException("Cannot display empty text when prompting for a choice");
        }

        return await onChoice(cancellationToken).ConfigureWithUnrealContext();
    }

    [UFunction]
    private void OnAdvanceAction()
    {
        if (!AwaitingInput || !DialogueBox.HasFinishedPlayingLine)
            return;

        OnAdvance?.Invoke();
        AwaitingInput = false;
    }
}
