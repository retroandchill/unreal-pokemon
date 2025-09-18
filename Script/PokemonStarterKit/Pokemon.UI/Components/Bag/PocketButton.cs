using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.GameplayTags;

namespace Pokemon.UI.Components.Bag;

[UClass]
public class UPocketButton : UCommonButtonBase
{
    public FGameplayTag Pocket { get; set; }
}
