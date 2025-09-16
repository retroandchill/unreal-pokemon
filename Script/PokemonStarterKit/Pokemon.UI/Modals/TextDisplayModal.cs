using InteractiveUI.Core.Dialogue;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Modals;

[UClass]
public class UTextDisplayModal : UCommonActivatableWidget
{
    [UProperty]
    [BindWidget]
    [UsedImplicitly]
    private UDialogueDisplayWidget MessageBox { get; }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Message")]
    public Task DisplayMessage(FText text, CancellationToken cancellationToken = default)
    {
        return MessageBox.DisplayDialogue(text, cancellationToken);
    }
}