using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UPokemonIconDisplay : UPokemonDisplayBase
{
    [UProperty]
    [BindWidget]
    private UEnhancedImage Image { get; }
    
    public override void Refresh()
    {
        if (Pokemon is null)
        {
            Image.Visibility = ESlateVisibility.Hidden;
            return;
        }
        
        var icon = UPokemonAssetLoader.ResolvePokemonIcon(Pokemon);
        Image.SetBrushFromLazyPaperFlipbook(icon, true);
        Image.Visibility = ESlateVisibility.SelfHitTestInvisible;
    }
}