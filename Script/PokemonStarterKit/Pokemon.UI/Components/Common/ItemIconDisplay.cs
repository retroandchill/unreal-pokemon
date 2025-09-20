using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UItemIconDisplay : UItemDisplayBase
{
    [UProperty]
    [BindWidget]
    private UEnhancedImage Image { get; }

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
