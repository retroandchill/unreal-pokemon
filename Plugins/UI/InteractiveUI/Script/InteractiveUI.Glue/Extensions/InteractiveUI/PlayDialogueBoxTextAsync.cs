using UnrealSharp.Core;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UnrealSharpAsync;

namespace UnrealSharp.InteractiveUI;

internal partial class UPlayDialogueBoxTextAsync
{
    private readonly TaskCompletionSource _tcs = new();

    private readonly Action _onAsyncCompleted;

    internal UPlayDialogueBoxTextAsync()
    {
        _onAsyncCompleted = OnAsyncCompleted;
    }

    private void OnAsyncCompleted()
    {
        _tcs.SetResult();
    }

    public static Task PlayDialogueBoxTextAsync(
        UDialogueBox dialogueBox,
        FText text,
        CancellationToken cancellationToken = default
    )
    {
        var async = NewObject<UPlayDialogueBoxTextAsync>(dialogueBox);
        NativeAsyncUtilities.InitializeAsyncAction(async, async._onAsyncCompleted);
        async.PlayDialogueBoxText(dialogueBox, text);
        cancellationToken.Register(dialogueBox.SkipToLineEnd);
        return async._tcs.Task;
    }
}
