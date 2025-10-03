using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// Represents a display widget for a Pokémon's sprite, including front or back views.
/// Inherits from <see cref="UPokemonDisplayBase"/>.
/// </summary>
[UClass(ClassFlags.Abstract)]
public class UPokemonSpriteDisplay : UPokemonDisplayBase
{
    [UProperty]
    [BindWidget]
    private UEnhancedImage Image { get; }

    /// <summary>
    /// Gets or sets a value indicating whether the back sprite of the Pokémon should be displayed.
    /// </summary>
    /// <remarks>
    /// When set to true, the back sprite of the Pokémon will be resolved and displayed using the
    /// <see cref="UPokemonAssetLoader.ResolvePokemonBackSprite(UPokemon)"/> method. When false,
    /// the front sprite will be resolved and displayed using
    /// <see cref="UPokemonAssetLoader.ResolvePokemonFrontSprite(UPokemon)"/>.
    /// Changing this property will automatically refresh the display to show the appropriate sprite.
    /// </remarks>
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

    /// <summary>
    ///
    /// </summary>
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
