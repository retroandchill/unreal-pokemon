using UnrealSharp;
using UnrealSharp.InteractiveUI;

namespace InteractiveUI.Core.Dialogue;

public static class DialogueBoxExtensions
{
    /// <summary>
    /// Asynchronously plays a line of dialogue within the provided dialogue box.
    /// </summary>
    /// <param name="dialogueBox">The dialogue box in which the line will be played.</param>
    /// <param name="line">The text of the dialogue line to play.</param>
    /// <param name="cancellationToken">An optional cancellation token to cancel the operation.</param>
    /// <returns>A task representing the asynchronous operation of playing the dialogue line.</returns>
    public static Task PlayLineAsync(
        this UDialogueBox dialogueBox,
        FText line,
        CancellationToken cancellationToken = default
    )
    {
        return UPlayDialogueBoxTextAsync.PlayDialogueBoxTextAsync(
            dialogueBox,
            line,
            cancellationToken
        );
    }
}
