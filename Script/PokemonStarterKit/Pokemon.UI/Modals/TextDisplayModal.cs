using InteractiveUI.Core.Dialogue;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Core;

namespace Pokemon.UI.Modals;

/// <summary>
/// Represents a text display modal UI widget, which is a subclass of UCommonActivatableWidget.
/// This widget is designed to display a text-based message to the user.
/// </summary>
[UClass]
public partial class UTextDisplayModal : UCommonActivatableWidget
{
    [UProperty]
    [BindWidget]
    private partial UDialogueDisplayWidget MessageBox { get; }

    /// <summary>
    /// Asynchronously displays a text message in the modal.
    /// </summary>
    /// <param name="text">The text to display in the modal dialog.</param>
    /// <param name="cancellationToken">An optional cancellation token to handle task cancellation.</param>
    /// <returns>A task that represents the asynchronous operation of displaying the message.</returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Message")]
    public Task DisplayMessage(FText text, CancellationToken cancellationToken = default)
    {
        return MessageBox.DisplayDialogue(text, cancellationToken);
    }
}
