using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Represents a UI component for displaying an item's icon.
/// This class extends the functionality of <see cref="UItemDisplayBase"/>
/// and is specifically designed to visually represent the associated item's icon.
/// </summary>
/// <remarks>
/// If the associated item is invalid, the icon will be hidden.
/// Otherwise, the item's icon will be loaded and displayed
/// using a brush texture.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public class UItemIconDisplay : UItemDisplayBase
{
    [UProperty]
    [BindWidget]
    private UEnhancedImage Image { get; }

    /// <inheritdoc />
    public override void Refresh()
    {
        if (!Item.IsValid)
        {
            Image.Visibility = ESlateVisibility.Hidden;
            return;
        }

        var icon = UIconAssetLoader.ResolveItemIcon(Item);
        Image.SetBrushFromLazyTexture(icon, true);
        Image.Visibility = ESlateVisibility.SelfHitTestInvisible;
    }
}
