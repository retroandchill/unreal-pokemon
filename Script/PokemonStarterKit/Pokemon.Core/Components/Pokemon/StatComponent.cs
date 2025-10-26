using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Data;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using ZLinq;

namespace Pokemon.Core.Components.Pokemon;

public static class TempDeconstructForcer
{
    public static void Deconstruct(this StructView<FNatureStatChange> stats, out FMainBattleStatHandle stat, out int value)
    {
        (stat, value) = stats.Copy();
    }
}

/// <summary>
/// The <c>UStatComponent</c> class manages the statistical attributes of a Pokemon entity.
/// This includes base stats, intermediate calculations (such as IVs, EVs, and nature),
/// and the ability to manipulate level, experience, and other statistics in the context of RPG mechanics.
/// </summary>
/// <remarks>
/// The component implements both <c>IBattleCapableComponent</c> and <c>IHealableComponent</c> interfaces,
/// ensuring it can be used in combat and healing-related workflows.
/// </remarks>
/// <see cref="URPGComponent"/>
/// <see cref="IBattleCapableComponent"/>
/// <see cref="IHealableComponent"/>
[UClass]
[UMetaData("HideCategories", "Stats")]
public partial class UStatComponent : URPGComponent, IBattleCapableComponent, IHealableComponent
{
    /// <summary>
    /// Represents the maximum allowable Individual Value (IV) for a Pokémon's stats.
    /// IVs are a numerical variable in Pokémon games that influence the base stat growth
    /// of a Pokémon. This value, defined as a constant, is used to cap the randomized or assigned
    /// IVs within a permissible range. The maximum IV in this system is set to 31.
    /// </summary>
    [PublicAPI]
    public const int MaxIV = 31;

    /// <summary>
    /// Represents the maximum allowable Effort Value (EV) that can be assigned to an individual stat
    /// of a Pokémon. EVs contribute to improving a Pokémon's base stats and are acquired through
    /// battles or specific items. The maximum EV per stat is capped at 252 in this system.
    /// </summary>
    [PublicAPI]
    public const int MaxEV = 252;

    [UProperty(PropertyFlags.Transient)]
    private partial UIdentityComponent IdentityComponent { get; set; }

    /// <summary>
    /// Represents the current level of a Pokémon.
    /// The level determines a Pokémon's overall growth and strength, including its base stats,
    /// accessible moves, and experience thresholds required to progress further.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int Level { get; private set; }

    /// <summary>
    /// Represents the experience points a Pokémon has accumulated.
    /// Experience (Exp) determines a Pokémon's level and progression in terms of strength and abilities.
    /// It increases as the Pokémon participates in battles, and upon reaching specific thresholds,
    /// it contributes to leveling up.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Exp")]
    public partial int Exp { get; private set; }

    /// <summary>
    /// Determines the amount of experience points required for the Pokémon to reach the next level.
    /// This value is calculated based on the species' growth rate and the current level.
    /// If the Pokémon is at the maximum level, this property returns 0 as no more experience is needed.
    /// </summary>
    public int ExpForNextLevel
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Exp")]
        get
        {
            if (Level == FGrowthRate.MaxLevel)
                return 0;

            return PokemonStatics
                .GetExpGrowthFormula(IdentityComponent.Species.Entry.GrowthRate)
                .GetMinimumExpForLevel(Level + 1);
        }
    }

    /// <summary>
    /// Represents the current progress towards the next level as a percentage.
    /// This value is calculated based on the Pokémon's current experience points
    /// relative to the experience required for the current and next levels.
    /// If the Pokémon is already at the maximum level, this percentage is set to 0.0.
    /// </summary>
    public float ExpPercent
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Exp")]
        get
        {
            if (Level == FGrowthRate.MaxLevel)
                return 0.0f;

            var growthRate = PokemonStatics.GetExpGrowthFormula(IdentityComponent.Species.Entry.GrowthRate);
            var expNeededForLevel = growthRate.GetMinimumExpForLevel(Level);
            var totalNeededForLevel = growthRate.GetMinimumExpForLevel(Level + 1);
            return (float)(Exp - expNeededForLevel) / totalNeededForLevel;
        }
    }

    /// <summary>
    /// Represents the current Hit Points (HP) of a Pokémon. HP determines the Pokémon's ability
    /// to remain in battle. When this value reaches zero, the Pokémon is considered unable to fight
    /// until it is healed. This value can be adjusted during battles or through actions like healing.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int HP { get; set; }

    /// <summary>
    /// Represents the maximum health points (HP) a Pokémon can have.
    /// This value determines the upper limit of the Pokémon's HP stat and is recalculated
    /// based on level, base stats, individual values (IVs), effort values (EVs), and nature.
    /// It is a critical factor in determining a Pokémon's survivability during battles or other scenarios.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int MaxHP { get; private set; }

    /// <summary>
    /// Represents the Attack stat, which determines the physical damage a Pokémon
    /// can deal using physical moves during battles.
    /// This stat is influenced by factors such as base stats, level, Individual Values (IVs),
    /// Effort Values (EVs), and the Pokémon's Nature.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int Attack { get; private set; }

    /// <summary>
    /// Represents the Defense stat of a Pokémon, which determines its ability to reduce
    /// the damage taken from physical attacks. This property is recalculated dynamically
    /// during stat evaluations and is influenced by the Pokémon's base stats, level, IV (Individual Values),
    /// EV (Effort Values), and nature modifications.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int Defense { get; private set; }

    /// <summary>
    /// Represents the Special Attack stat of a Pokémon, used to determine the power of special moves during battles.
    /// This stat influences damage calculations for attacks that are dependent on special capabilities
    /// rather than physical strength. The value is calculated based on the Pokémon's base stats, Individual Values (IVs),
    /// Effort Values (EVs), Level, and nature modifiers.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int SpecialAttack { get; private set; }

    /// <summary>
    /// Represents the Special Defense stat of a Pokémon.
    /// This stat determines the ability of a Pokémon to reduce damage
    /// taken from special attacks during battles. It is influenced
    /// by factors such as base stats, Individual Values (IVs),
    /// Effort Values (EVs), level, and nature modifiers.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int SpecialDefense { get; private set; }

    /// <summary>
    /// Represents the Speed stat of a Pokémon, which determines the order of actions
    /// in battles. A higher Speed value allows the Pokémon to act earlier compared to
    /// opponents with lower Speed. This value is calculated based on the Pokémon's
    /// base stats, level, IVs, EVs, and Nature modifiers.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial int Speed { get; private set; }

    [UProperty]
    private partial TMap<FMainStatHandle, int> IV { get; }

    [UProperty]
    private partial TMap<FMainStatHandle, int> IVOverrides { get; }

    /// <summary>
    /// Provides the effective Individual Values (IVs) for each main stat of a Pokémon.
    /// Effective IVs are determined by applying any overrides specified for the Pokémon's stats.
    /// If no override exists for a particular stat, the default IV for that stat is used.
    /// The resulting effective IV values are utilized during stat calculations, ensuring that
    /// the Pokémon's stats accurately reflect any modifications or customizations made.
    /// </summary>
    public IReadOnlyDictionary<FMainStatHandle, int> EffectiveIVs
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get
        {
            var effectiveIVs = new Dictionary<FMainStatHandle, int>();
            foreach (var (stat, _) in FMainStatHandle.Entries)
            {
                effectiveIVs.Add(stat, IVOverrides.TryGetValue(stat, out var value) ? value : IV[stat]);
            }

            return effectiveIVs;
        }
    }

    [UProperty]
    private partial TMap<FMainStatHandle, int> EV { get; }

    /// <summary>
    /// Represents the Nature of a Pokémon, which influences its stat growth by boosting one stat
    /// while hindering another. Natures impact the overall performance and behavior of a Pokémon
    /// by subtly altering its base stat calculations, contributing to its individual characteristics
    /// within the game.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial FNatureHandle Nature { get; set; }

    /// <summary>
    /// Represents an override for the Pokémon's default nature.
    /// The Nature determines how certain stats are influenced, typically providing
    /// a boost to one stat while reducing another. If set, this property will take
    /// precedence over the Pokémon's default nature, modifying its stat modifiers accordingly.
    /// The value is optional and must be of type <see cref="FNatureHandle"/>.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public partial TOptional<FNatureHandle> NatureOverride { get; set; }

    /// <summary>
    /// Represents the effective nature applied to the Pokémon's stats.
    /// The effective nature determines how certain stats are influenced positively or negatively.
    /// If a nature override is present, it takes precedence; otherwise, the Pokémon's default nature is used.
    /// </summary>
    public FNatureHandle EffectiveNature
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get { return NatureOverride.Match(x => x, () => Nature); }
    }

    /// <inheritdoc />
    [ExcludeFromExtensions]
    public bool IsAbleToBattle => HP > 0;

    /// <summary>
    /// Indicates whether the Pokémon has fainted. A Pokémon is considered fainted
    /// when its current HP is less than or equal to zero.
    /// </summary>
    public bool IsFainted
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get => HP <= 0;
    }

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    /// <summary>
    /// Initializes the stat component of the Pokémon using the provided initialization parameters.
    /// </summary>
    /// <param name="initParams">The initialization parameters containing the level and species data for the Pokémon.</param>
    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        Level = initParams.Level;

        var natureIndex = (int)(IdentityComponent.PersonalityValue % GameData.Natures.Entries.Length);
        Nature = GameData.Natures.Entries[natureIndex];

        foreach (var (stat, _) in FMainStatHandle.Entries)
        {
            var handle = new FMainStatHandle(stat);
            IV.Add(handle, Random.Shared.Next(MaxIV + 1));
            EV.Add(handle, 0);
        }

        RecalculateStats();
    }

    /// <summary>
    /// Sets the level of the Pokémon and optionally recalculates its stats.
    /// </summary>
    /// <param name="level">The desired level to set for the Pokémon. This value will be clamped to a valid range defined by the maximum level.</param>
    /// <param name="recalculate">Indicates whether stats should be recalculated after setting the level. Default is true.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetLevel(int level, bool recalculate = true)
    {
        Level = Math.Clamp(level, 1, FGrowthRate.MaxLevel);
        Exp = PokemonStatics
            .GetExpGrowthFormula(IdentityComponent.Species.Entry.GrowthRate)
            .GetMinimumExpForLevel(Level);
        if (recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Sets the experience points of the stat component, adjusting the value within valid bounds,
    /// and recalculates the Pokemon's stats if the level changes.
    /// </summary>
    /// <param name="exp">The new experience points to set for the stat component.</param>
    /// <param name="recalculate">
    /// Indicates whether stats should be recalculated if there is a level change. Defaults to true.
    /// </param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetExp(int exp, bool recalculate = true)
    {
        Exp = Math.Clamp(exp, 0, ExpForNextLevel);
        var oldLevel = Level;
        Level = PokemonStatics.GetExpGrowthFormula(IdentityComponent.Species.Entry.GrowthRate).GetLevelForExp(Exp);
        if (oldLevel != Level && recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Retrieves the Individual Value (IV) for the specified stat.
    /// </summary>
    /// <param name="stat">The stat handle for which to retrieve the IV value.</param>
    /// <returns>The IV value associated with the specified stat.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
    public int GetIV(FMainStatHandle stat) => IV[stat];

    /// <summary>
    /// Sets the Individual Value (IV) for a specified main stat handle.
    /// </summary>
    /// <param name="stat">The handle representing the stat whose IV is to be set.</param>
    /// <param name="value">The IV to set, clamped between 0 and the maximum allowable IV value.</param>
    /// <param name="recalculate">
    /// Indicates whether to recalculate the related stats after setting the IV.
    /// Defaults to true if not explicitly specified.
    /// </param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetIV(FMainStatHandle stat, int value, bool recalculate = true)
    {
        IV[stat] = Math.Clamp(value, 0, MaxIV);
        if (recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Retrieves the Individual Value (IV) override for a given stat handle if it exists.
    /// </summary>
    /// <param name="stat">The stat handle for which the IV override value is requested.</param>
    /// <returns>
    /// An optional integer representing the IV override value if found; otherwise, an empty option.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Get IV Override", Category = "Stats")]
    public TOptional<int> GetIVOverride(FMainStatHandle stat)
    {
        return IVOverrides.TryGetValue(stat, out var value) ? value : TOptional<int>.None;
    }

    /// <summary>
    /// Sets an Individual Value (IV) override for the specified stat.
    /// </summary>
    /// <param name="stat">The stat to override.</param>
    /// <param name="value">The new IV value for the specified stat.</param>
    /// <param name="recalculate">
    /// Determines whether to recalculate the stats after the override is set.
    /// Default is true.
    /// </param>
    [UFunction(FunctionFlags.BlueprintCallable, DisplayName = "Set IV Override", Category = "Stats")]
    public void SetIVOverride(FMainStatHandle stat, int value, bool recalculate = true)
    {
        IVOverrides.Add(stat, value);
        if (recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Removes an Individual Value (IV) override associated with the specified stat.
    /// Optionally recalculates the stats after the override is cleared.
    /// </summary>
    /// <param name="stat">The stat handle corresponding to the stat whose IV override is to be cleared.</param>
    /// <param name="recalculate">
    /// A boolean value indicating whether the stats should be recalculated after clearing the override.
    /// Defaults to true.
    /// </param>
    [UFunction(FunctionFlags.BlueprintCallable, DisplayName = "Clear IV Override", Category = "Stats")]
    public void ClearIVOverride(FMainStatHandle stat, bool recalculate = true)
    {
        IVOverrides.Remove(stat);
        if (recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Retrieves the Effort Value (EV) for the specified stat.
    /// </summary>
    /// <param name="stat">The stat handle for which the Effort Value is to be obtained.</param>
    /// <returns>The Effort Value associated with the specified stat.</returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
    public int GetEV(FMainStatHandle stat) => EV[stat];

    /// <summary>
    /// Sets the EV (Effort Value) for a specific stat.
    /// </summary>
    /// <param name="stat">The stat for which the EV should be set.</param>
    /// <param name="value">The EV value to set. The value will be clamped between 0 and the maximum allowed EV.</param>
    /// <param name="recalculate">Indicates whether to recalculate the stats after setting the EV. Defaults to true.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetEV(FMainStatHandle stat, int value, bool recalculate = true)
    {
        EV[stat] = Math.Clamp(value, 0, MaxEV);
        if (recalculate)
            RecalculateStats();
    }

    /// <summary>
    /// Recalculates the stats of the Pokémon using its base stats, level, IVs, EVs, and nature modifications.
    /// This method ensures that the stats are updated to reflect any changes to individual components like level, IVs, nature, or other contributing factors.
    /// Adjusts the Pokémon's HP to account for changes in the maximum HP, ensuring the current HP remains valid.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void RecalculateStats()
    {
        var baseStats = IdentityComponent.Species.Entry.BaseStats;
        var ivs = EffectiveIVs;

        var natureMod = FMainStatHandle.Entries.ToDictionary(x => x.ID, _ => 100);

        var effectiveNature = EffectiveNature;
        if (effectiveNature.IsValid)
        {
            foreach (var (stat, change) in effectiveNature.Entry.StatChanges)
            {
                natureMod[stat] += change;
            }
        }

        var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
        var stats = FMainStatHandle.Entries.ToDictionary(
            x => x.ID,
            x =>
                subsystem.StatCalculationService.CalculateStat(
                    x.ID,
                    baseStats[x.ID],
                    Level,
                    ivs[x.ID],
                    EV[x.ID],
                    natureMod[x.ID]
                )
        );

        var hpDifference = stats[FStat.HP] - MaxHP;
        MaxHP = stats[FStat.HP];
        if (HP > 0 || hpDifference > 0)
            HP = Math.Max(1, HP + hpDifference);
        Attack = stats[FStat.ATTACK];
        Defense = stats[FStat.DEFENSE];
        SpecialAttack = stats[FStat.SPECIAL_ATTACK];
        SpecialDefense = stats[FStat.SPECIAL_DEFENSE];
        Speed = stats[FStat.SPEED];
    }

    /// <inheritdoc />
    [ExcludeFromExtensions]
    public void Heal()
    {
        HP = MaxHP;
    }
}
