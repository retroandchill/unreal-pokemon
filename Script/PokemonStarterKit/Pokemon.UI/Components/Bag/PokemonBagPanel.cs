using Pokemon.Core.Entities;
using Pokemon.UI.Components.Common;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Bag;

/// <summary>
/// Represents a UI panel used to display information about a Pokémon in the player's bag.
/// </summary>
/// <remarks>
/// This class extends <see cref="UPokemonButtonBase"/> and provides additional functionality for displaying
/// a Pokémon's HP status, including a progress bar and a text block to show current and maximum HP.
/// </remarks>
[UClass]
public partial class UPokemonBagPanel : UPokemonButtonBase
{
    [UProperty]
    [BindWidget]
    private partial UPokemonDisplayBase PokemonIcon { get; }

    /// <summary>
    /// Represents the health progress bar displaying the current percentage of a Pokémon's HP.
    /// </summary>
    /// <remarks>
    /// The <c>HPBar</c> dynamically visualizes the Pokémon's health status. The progress is calculated as
    /// a percentage of the Pokémon's current HP relative to its maximum HP. This enables a clear and intuitive
    /// representation of the Pokémon's vitality in the UI.
    /// </remarks>
    [UProperty]
    [BindWidget]
    protected partial UProgressBar HPBar { get; }

    /// <summary>
    /// Represents a text block displaying the current and maximum HP of a Pokémon in the player's bag.
    /// </summary>
    /// <remarks>
    /// The <c>HPText</c> is dynamically updated to show the Pokémon's health as text in the format "Current HP / Maximum HP".
    /// It complements the <c>HPBar</c>, providing a precise numerical representation of the Pokémon's vitality.
    /// </remarks>
    [UProperty]
    [BindWidget]
    protected partial UCommonTextBlock HPText { get; }

    /// <inheritdoc />
    public override void Refresh_Implementation()
    {
        PokemonIcon.Pokemon = Pokemon;

        if (Pokemon is null)
            return;

        HPText.Text = $"{Pokemon.HP}/{Pokemon.MaxHP}";
        HPBar.Percent = Pokemon.HP / (float)Pokemon.MaxHP;
    }
}
