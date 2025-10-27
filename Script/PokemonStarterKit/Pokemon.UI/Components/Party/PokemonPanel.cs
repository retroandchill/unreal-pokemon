using Pokemon.Assets;
using Pokemon.Core;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.UI.Components.Common;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.CommonUI;
using UnrealSharp.Engine;
using UnrealSharp.InteractiveUI;
using UnrealSharp.UMG;

namespace Pokemon.UI.Components.Party;

/// <summary>
/// Represents a UI panel for a Pokémon in the party interface, which extends from UPokemonButtonBase.
/// The panel provides various information and states related to the Pokémon, including its name, level,
/// gender, health, and status effects, and includes behavior for swapping and selection functionality.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UPokemonPanel : UPokemonButtonBase
{
    [UProperty]
    [BindWidget]
    private partial UPokemonDisplayBase PokemonIcon { get; }

    [UProperty]
    [BindWidget]
    private partial UItemDisplayBase ItemIcon { get; }

    [UProperty]
    [BindWidget]
    protected partial UCommonTextBlock NameText { get; }

    [UProperty]
    [BindWidget]
    protected partial UCommonNumericTextBlock LevelText { get; }

    [UProperty]
    [BindWidget]
    protected partial UEnhancedImage GenderIcon { get; }

    /// <summary>
    /// Represents a mapping of Pokémon genders to their respective visual icon representations.
    /// The dictionary keys are of type <see cref="EPokemonGender"/>, corresponding to the gender
    /// of the Pokémon (Male, Female, Genderless), and the values are textures of type
    /// <see cref="UTexture2D"/> used to display the appropriate gender-related icon.
    /// </summary>
    /// <remarks>
    /// This property is utilized to dynamically assign the correct gender icon
    /// for a Pokémon based on its gender during UI rendering within a Pokémon panel.
    /// </remarks>
    [UProperty(PropertyFlags.EditDefaultsOnly, Category = "Display")]
    private partial TMap<EPokemonGender, UTexture2D> GenderIcons { get; }

    [UProperty]
    [BindWidget]
    protected partial UProgressBar HPBar { get; }

    [UProperty]
    [BindWidget]
    protected partial UCommonTextBlock HPText { get; }

    [UProperty]
    [BindWidget]
    protected partial UEnhancedImage StatusEffectIcon { get; }

    /// <summary>
    /// Indicates whether this Pokémon panel is currently active, corresponding to the Pokémon
    /// displayed in the lead position of the player's party.
    /// </summary>
    /// <remarks>
    /// This property checks if the Pokémon represented by this panel matches the first Pokémon
    /// in the player's party as determined by the game's Pokémon subsystem.
    /// It is primarily used to adjust UI states or behaviors that depend on the leading
    /// Pokémon's status in the player's party, such as highlighting the active Pokémon panel.
    /// </remarks>
    public bool IsActive
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Active", Category = "Display")]
        get
        {
            var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
            return ReferenceEquals(subsystem.Player.PartyPokemon[0], Pokemon);
        }
    }

    /// <summary>
    /// Indicates whether the Pokémon associated with this panel is in a fainted state.
    /// This property evaluates to <c>true</c> if the Pokémon is present and its
    /// <see cref="UPokemon.IsFainted"/> property is set to <c>true</c>; otherwise, it evaluates to <c>false</c>.
    /// </summary>
    /// <remarks>
    /// This property is used to determine the display and interaction status within the
    /// UI panel, often affecting visual elements such as health bars and status indicators.
    /// </remarks>
    public bool IsPokemonFainted
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Pokémon Fainted", Category = "Display")]
        get => Pokemon is not null && Pokemon.IsFainted;
    }

    /// <summary>
    /// Determines whether the Pokémon panel is currently selected in the user interface.
    /// This property returns a boolean value indicating the panel's selected state,
    /// which corresponds to its hover status.
    /// </summary>
    /// <remarks>
    /// Utilized to visually and logically represent the active selection within the Pokémon party interface.
    /// When true, the panel is considered as selected, providing feedback for hover-based interactions.
    /// </remarks>
    public bool IsPanelSelected
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Panel Selected", Category = "Display")]
        get => IsHovered();
    }

    /// <summary>
    /// Indicates whether the current Pokémon panel is in a swapping state within the party interface.
    /// </summary>
    /// <remarks>
    /// This property is determined by checking if the parent selection pane, represented as <see cref="UPokemonSelectionPane"/>,
    /// is currently in the process of switching Pokémon (i.e., <see cref="UPokemonSelectionPane.IsSwitching"/> is true).
    /// It is used to manage visual and interactive states when a Pokémon swap is being performed.
    /// </remarks>
    public bool IsSwapping
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Swapping", Category = "Display")]
        get => Outer is UPokemonSelectionPane { IsSwitching: true };
    }

    /// <summary>
    /// Determines whether the current Pokémon panel is marked as preselected for swapping within a party interface.
    /// A panel is considered preselected when it matches the Pokémon at the switching index in the player's party,
    /// during an active switching operation in the parent <see cref="UPokemonSelectionPane"/>.
    /// </summary>
    /// <remarks>
    /// This property is utilized to provide a visual indication or logic handling for preselected Pokémon during
    /// swap operations. The determination relies on the current state of the selection pane and the party's Pokémon lineup.
    /// </remarks>
    public bool IsPreselected
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Preselected", Category = "Display")]
        get
        {
            if (Outer is not UPokemonSelectionPane { IsSwitching: true } selectionPane)
                return false;
            var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
            return ReferenceEquals(subsystem.Player.PartyPokemon[selectionPane.SwitchingIndex.Value], Pokemon);
        }
    }

    /// <summary>
    /// Swaps the Pokémon between the current panel and the specified panel.
    /// </summary>
    /// <param name="other">The other Pokémon panel to swap with.</param>
    public void SwapPokemon(UPokemonPanel other)
    {
        (Pokemon, other.Pokemon) = (other.Pokemon, Pokemon);
    }

    /// <inheritdoc />
    public override void Refresh_Implementation()
    {
        PokemonIcon.Pokemon = Pokemon;

        if (Pokemon is null)
            return;

        Pokemon.Item.Match(
            i =>
            {
                ItemIcon.Item = i;
                ItemIcon.Visibility = ESlateVisibility.SelfHitTestInvisible;
            },
            () =>
            {
                ItemIcon.Visibility = ESlateVisibility.Hidden;
            }
        );
        NameText.Text = Pokemon.Nickname;
        LevelText.CurrentValue = Pokemon.Level;

        if (GenderIcons.TryGetValue(Pokemon.Gender, out var icon))
        {
            GenderIcon.Visibility = ESlateVisibility.SelfHitTestInvisible;
            GenderIcon.SetBrushFromTexture(icon, true);
        }
        else
        {
            GenderIcon.Visibility = ESlateVisibility.Hidden;
        }

        HPText.Text = $"{Pokemon.HP}/{Pokemon.MaxHP}";
        HPBar.Percent = Pokemon.HP / (float)Pokemon.MaxHP;

        if (Pokemon.IsFainted)
        {
            StatusEffectIcon.Visibility = ESlateVisibility.SelfHitTestInvisible;
            var assetManager = UAssetManager.Get();
            var faintedIcon = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(AssetIds.StatusIcons_icon_faint);
            StatusEffectIcon.SetBrushFromLazyTexture(faintedIcon.Cast<UTexture2D>(), true);
        }
        else
        {
            Pokemon.StatusEffect.Match(
                s =>
                {
                    StatusEffectIcon.Visibility = ESlateVisibility.SelfHitTestInvisible;
                    var statusIcon = UIconAssetLoader.ResolveStatusEffectIcon(s);
                    StatusEffectIcon.SetBrushFromLazyTexture(statusIcon, true);
                },
                () =>
                {
                    StatusEffectIcon.Visibility = ESlateVisibility.Hidden;
                }
            );
        }
    }
}
