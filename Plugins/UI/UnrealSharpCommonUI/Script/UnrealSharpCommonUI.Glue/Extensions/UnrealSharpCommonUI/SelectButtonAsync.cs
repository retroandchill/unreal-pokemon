using UnrealSharp.CommonUI;
using UnrealSharp.UnrealSharpAsync;

namespace UnrealSharp.UnrealSharpCommonUI;

public readonly record struct OptionSelection(UCommonButtonBase Button, int Index);

internal partial class USelectButtonAsync
{
    private readonly TaskCompletionSource<OptionSelection> _tcs = new();

    private readonly Action _onAsyncCompleted;

    internal USelectButtonAsync()
    {
        _onAsyncCompleted = OnAsyncCompleted;
    }

    private void OnAsyncCompleted()
    {
        _tcs.SetResult(new OptionSelection(SelectedButton, SelectedButtonIndex));
    }

    public static Task<OptionSelection> SelectButtonAsync(
        UCommonButtonGroupBase buttonGroup,
        CancellationToken cancellationToken = default
    )
    {
        var waiter = NewObject<USelectButtonAsync>(buttonGroup);
        NativeAsyncUtilities.InitializeAsyncAction(waiter, waiter._onAsyncCompleted);
        waiter.SelectButton(buttonGroup);
        cancellationToken.Register(waiter.CancelSelection);
        return waiter._tcs.Task;
    }
}

/// <summary>
/// Provides extension methods for asynchronous selection functionality in a UCommonButtonGroupBase.
/// </summary>
public static class SelectButtonAsyncExtensions
{
    /// <summary>
    /// Asynchronously waits for a button to be selected within the specified button group and returns the selected button along with its index.
    /// </summary>
    /// <param name="buttonGroup">The button group containing the buttons to select from.</param>
    /// <param name="cancellationToken">An optional cancellation token to cancel the selection operation.</param>
    /// <returns>A task that represents the asynchronous operation. The task result contains the selected button and its index.</returns>
    public static Task<OptionSelection> SelectButtonAsync(
        this UCommonButtonGroupBase buttonGroup,
        CancellationToken cancellationToken = default
    )
    {
        return USelectButtonAsync.SelectButtonAsync(buttonGroup, cancellationToken);
    }
}
