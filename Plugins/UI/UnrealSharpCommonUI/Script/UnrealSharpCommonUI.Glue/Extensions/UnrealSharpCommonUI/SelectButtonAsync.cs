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

public static class SelectButtonAsyncExtensions
{
    public static Task<OptionSelection> SelectButtonAsync(
        this UCommonButtonGroupBase buttonGroup,
        CancellationToken cancellationToken = default
    )
    {
        return USelectButtonAsync.SelectButtonAsync(buttonGroup, cancellationToken);
    }
}
