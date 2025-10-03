using JetBrains.Annotations;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Bag;

/// <summary>
/// Represents a specific category, or "pocket," within a bag inventory system.
/// The pocket is linked to a specific tag and has a name, maximum slots,
/// and maximum stack size for items.
/// </summary>
/// <remarks>
/// This class is a specialized implementation of <see cref="UInventoryContainer"/>.
/// It provides mechanisms to validate items based on their associated pocket tags,
/// ensuring that only valid items can be stored within this pocket.
/// </remarks>
[UClass]
[UsedImplicitly]
public class UBagPocket : UInventoryContainer
{
    /// <summary>
    /// Represents the gameplay tag associated with this bag pocket.
    /// </summary>
    /// <remarks>
    /// This tag serves as a unique identifier for the pocket and is used to categorize
    /// and validate items that can be stored within this pocket. The tag ensures that only
    /// items matching the tag's associated category can be added to the pocket.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Inventory")]
    public FGameplayTag Tag { get; internal set; }

    /// <summary>
    /// Represents the display name of the bag pocket within the inventory system.
    /// </summary>
    /// <remarks>
    /// The name provides a user-friendly identifier for the pocket, used in UI displays or other interactions.
    /// This property is typically localized to support different languages and enhance accessibility.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Display")]
    public FText Name { get; internal set; }

    /// <inheritdoc />
    protected override EInventoryValidation ValidateItem(FName item)
    {
        var handle = new FItemHandle(item);
        return handle.IsValid && Tag == handle.Entry.Pocket ? EInventoryValidation.Valid : EInventoryValidation.Invalid;
    }
}
