using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

[UClass(ClassFlags.Abstract)]
public class UPokemonSpriteDisplay : UPokemonDisplayBase
{
    [UProperty]
    [BindWidget]
    private UEnhancedImage Image { get; }

    [UProperty(PropertyFlags.EditAnywhere | PropertyFlags.BlueprintReadWrite, Category = "Display")]
    public bool ShowBackSprite
    {
        get;
        set
        {
            field = value;
            Refresh();
        }
    }

    public override void Refresh()
    {
        if (Pokemon is null)
        {
            Image.Visibility = ESlateVisibility.Hidden;
            return;
        }

        var sprite = ShowBackSprite
            ? UPokemonAssetLoader.ResolvePokemonBackSprite(Pokemon)
            : UPokemonAssetLoader.ResolvePokemonFrontSprite(Pokemon);
        Image.SetBrushFromLazySimpleFlipbook(sprite, true);
        Image.Visibility = ESlateVisibility.SelfHitTestInvisible;
    }
}
