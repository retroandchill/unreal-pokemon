using GameDataAccessTools.Core.Utilities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Battle;

/// <summary>
/// A class containing developer-configurable settings for Pokémon battle mechanics.
/// </summary>
/// <remarks>
/// The <c>UPokemonBattleSettings</c> class allows developers to configure various
/// aspects of Pokémon battle mechanics through editor defaults and configuration files.
/// This class primarily facilitates adjustments to the battle mechanics, including
/// those related to status effects such as paralysis.
/// </remarks>
/// <seealso cref="Pokemon.Battle.StatusEffects.ParalysisStatus"/>
/// <seealso cref="Pokemon.Data.Model.PBS.FAbilityHandle"/>
[UClass(ClassFlags.DefaultConfig, ConfigCategory = "Game")]
public partial class UPokemonBattleSettings : UCSDeveloperSettings
{
    /// <summary>
    /// A list of abilities that prevent the Speed drop caused by the paralysis status effect.
    /// </summary>
    /// <remarks>
    /// When a Pokémon affected by the paralysis status would normally experience a reduction
    /// in Speed, any abilities specified in this list will negate that reduction. This is particularly
    /// useful for configuring specific Pokémon abilities to counteract the drawbacks of paralysis.
    /// </remarks>
    /// <seealso cref="Pokemon.Battle.StatusEffects.ParalysisStatus"/>
    /// <seealso cref="Pokemon.Data.Model.PBS.FAbilityHandle"/>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Config,
        Category = "StatusEffects|Paralysis"
    )]
    public partial IReadOnlyList<FAbilityHandle> IgnoresParalysisSpeedDrop { get; }

    /// <inheritdoc />
    protected override FText GetSectionText_Implementation()
    {
        return FText.Localized("PokemonBattle", "PokemonBattle", "Pokémon Battle");
    }
}
