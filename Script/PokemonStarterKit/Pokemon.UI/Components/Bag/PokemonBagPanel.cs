using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

[UClass]
public class UPokemonBagPanel : UPokemonButtonBase
{
    [UProperty]
    [BindWidget]
    private UPokemonDisplayBase PokemonIcon { get; }

    [UProperty]
    [BindWidget]
    protected UProgressBar HPBar { get; }

    [UProperty]
    [BindWidget]
    protected UCommonTextBlock HPText { get; }

    public override void Refresh()
    {
        PokemonIcon.Pokemon = Pokemon;

        if (Pokemon is null)
            return;

        HPText.Text = $"{Pokemon.HP}/{Pokemon.MaxHP}";
        HPBar.Percent = Pokemon.HP / (float)Pokemon.MaxHP;
    }
}
