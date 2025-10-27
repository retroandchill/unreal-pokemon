using Pokemon.Core;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Core;
using UnrealSharp.GameplayTags;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents a UI window within the Pokemon Bag system that displays and manages pockets.
/// Inherits from the UCommonUserWidget, providing common user interface widget functionality.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UPocketWindow : UCommonUserWidget
{
    /// <summary>
    /// Gets or sets the currently selected pocket in the bag window.
    /// This property identifies the active pocket in the inventory system
    /// using a gameplay tag. Setting this property updates the associated
    /// pocket's UI details.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Inventory", BlueprintAccessors = true)]
    public FGameplayTag CurrentPocket
    {
        get => CurrentPocket_BackingField;
        set
        {
            CurrentPocket_BackingField = value;
            UpdatePocketInfo();
        }
    }

    [UProperty]
    [BindWidgetOptional]
    private partial UCommonTextBlock? PocketName { get; }

    private void UpdatePocketInfo()
    {
        PocketName?.Text =
            GetDefault<UPokemonCoreSettings>()
                .Pockets.Where(p => p.Tag == CurrentPocket)
                .Select(p => p.Name)
                .SingleOrDefault()
            ?? FText.None;
    }
}
