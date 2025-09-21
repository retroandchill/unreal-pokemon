using LanguageExt;
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

[UClass(ClassFlags.Abstract)]
public class UPokemonPanel : UPokemonButtonBase
{
    [UProperty]
    [BindWidget]
    private UPokemonDisplayBase PokemonIcon { get; }

    [UProperty]
    [BindWidget]
    private UItemDisplayBase ItemIcon { get; }

    [UProperty]
    [BindWidget]
    protected UCommonTextBlock NameText { get; }

    [UProperty]
    [BindWidget]
    protected UCommonNumericTextBlock LevelText { get; }

    [UProperty]
    [BindWidget]
    protected UEnhancedImage GenderIcon { get; }

    [UProperty(PropertyFlags.EditDefaultsOnly, Category = "Display")]
    private TMap<EPokemonGender, UTexture2D> GenderIcons { get; }

    [UProperty]
    [BindWidget]
    protected UProgressBar HPBar { get; }

    [UProperty]
    [BindWidget]
    protected UCommonTextBlock HPText { get; }

    [UProperty]
    [BindWidget]
    protected UEnhancedImage StatusEffectIcon { get; }

    public bool IsActivate
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Active", Category = "Display")]
        get
        {
            var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
            return ReferenceEquals(subsystem.Player.PartyPokemon[0], Pokemon);
        }
    }

    public bool IsPokemonFainted
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Pokémon Fainted",
            Category = "Display"
        )]
        get => Pokemon is not null && Pokemon.IsFainted;
    }

    public bool IsPanelSelected
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Panel Selected",
            Category = "Display"
        )]
        get => IsHovered();
    }

    public bool IsSwapping
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Swapping", Category = "Display")]
        get => Outer is UPokemonSelectionPane { IsSwitching: true };
    }

    public bool IsPreselected
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Preselected",
            Category = "Display"
        )]
        get
        {
            if (Outer is not UPokemonSelectionPane { IsSwitching: true } selectionPane)
                return false;
            var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
            return ReferenceEquals(
                subsystem.Player.PartyPokemon[selectionPane.SwitchingIndex.Value],
                Pokemon
            );
        }
    }

    public void SwapPokemon(UPokemonPanel other)
    {
        (Pokemon, other.Pokemon) = (other.Pokemon, Pokemon);
    }

    public override void Refresh()
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
            var faintedIcon = assetManager.GetSoftObjectReferenceFromPrimaryAssetId(
                AssetIds.StatusIcons_icon_faint
            );
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
