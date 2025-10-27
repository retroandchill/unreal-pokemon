using Pokemon.Assets;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Common;

/// <summary>
/// A class responsible for displaying a Pokémon's representative icon in the UI.
/// </summary>
/// <remarks>
/// UPokemonIconDisplay is a concrete implementation derived from UPokemonDisplayBase.
/// It specifically handles the logic for showing or hiding a Pokémon's icon and updating
/// the icon based on the associated Pokémon instance.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public partial class UPokemonIconDisplay : UPokemonDisplayBase
{
    [UProperty]
    [BindWidget]
    private partial UEnhancedImage Image { get; }

    /// <inheritdoc />
    public override void Refresh_Implementation()
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
