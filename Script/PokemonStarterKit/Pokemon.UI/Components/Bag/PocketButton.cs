using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;
using UnrealSharp.GameplayTags;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents a specialized button used for selecting different "pockets" within a UI, such as those found in a bag or inventory system.
/// </summary>
/// <remarks>
/// UPocketButton inherits from UCommonButtonBase, leveraging its base functionality for button interaction,
/// while adding additional behavior specific to its use case.
/// </remarks>
[UClass]
public partial class UPocketButton : UCommonButtonBase
{
    /// <summary>
    /// Represents the pocket tag associated with this UPocketButton.
    /// </summary>
    /// <remarks>
    /// The Pocket property identifies the specific "pocket" or category within an inventory or bag system
    /// that the button corresponds to. It is used to differentiate between different sections, such as items,
    /// medicine, or TMs. This is primarily utilized in conjunction with UI components to handle user interactions
    /// and selection changes.
    /// </remarks>
    public FGameplayTag Pocket { get; set; }
}
