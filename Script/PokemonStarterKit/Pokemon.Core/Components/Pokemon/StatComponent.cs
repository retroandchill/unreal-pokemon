using JetBrains.Annotations;
using LanguageExt;
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

[UClass]
[UMetaData("HideCategories", "Stats")]
[UsedImplicitly]
public class UStatComponent : URPGComponent, IBattleCapableComponent, IHealableComponent
{
    [PublicAPI]
    public const int MaxIV = 31;

    [PublicAPI]
    public const int MaxEV = 252;

    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int Level { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Exp")]
    public int Exp { get; private set; }

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

    public float ExpPercent
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Exp")]
        get
        {
            if (Level == FGrowthRate.MaxLevel)
                return 0.0f;

            var growthRate = PokemonStatics.GetExpGrowthFormula(
                IdentityComponent.Species.Entry.GrowthRate
            );
            var expNeededForLevel = growthRate.GetMinimumExpForLevel(Level);
            var totalNeededForLevel = growthRate.GetMinimumExpForLevel(Level + 1);
            return (float)(Exp - expNeededForLevel) / totalNeededForLevel;
        }
    }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int HP { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int MaxHP { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int Attack { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int Defense { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int SpecialAttack { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int SpecialDefense { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public int Speed { get; private set; }

    [UProperty]
    [UsedImplicitly]
    private TMap<FMainStatHandle, int> IV { get; }

    [UProperty]
    [UsedImplicitly]
    private TMap<FMainStatHandle, int> IVOverrides { get; }

    public IReadOnlyDictionary<FMainStatHandle, int> EffectiveIVs
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get
        {
            var effectiveIVs = new Dictionary<FMainStatHandle, int>();
            foreach (var (stat, _) in FMainStatHandle.Entries)
            {
                effectiveIVs.Add(
                    stat,
                    IVOverrides.TryGetValue(stat, out var value) ? value : IV[stat]
                );
            }

            return effectiveIVs;
        }
    }

    [UProperty]
    [UsedImplicitly]
    private TMap<FMainStatHandle, int> EV { get; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public FNatureHandle Nature { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Stats")]
    public Option<FNatureHandle> NatureOverride { get; set; }

    public FNatureHandle EffectiveNature
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get { return NatureOverride.Match(x => x, () => Nature); }
    }

    [ExcludeFromExtensions]
    public bool IsAbleToBattle => HP > 0;

    public bool IsFainted
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
        get => HP <= 0;
    }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        Level = initParams.Level;

        var natureIndex = (int)(
            IdentityComponent.PersonalityValue % GameData.Natures.Entries.Length
        );
        Nature = GameData.Natures.Entries[natureIndex];

        foreach (var (stat, _) in FMainStatHandle.Entries)
        {
            var handle = new FMainStatHandle(stat);
            IV.Add(handle, Random.Shared.Next(MaxIV + 1));
            EV.Add(handle, 0);
        }

        RecalculateStats();
    }

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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetExp(int exp, bool recalculate = true)
    {
        Exp = Math.Clamp(exp, 0, ExpForNextLevel);
        var oldLevel = Level;
        Level = PokemonStatics
            .GetExpGrowthFormula(IdentityComponent.Species.Entry.GrowthRate)
            .GetLevelForExp(Exp);
        if (oldLevel != Level && recalculate)
            RecalculateStats();
    }

    [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
    public int GetIV(FMainStatHandle stat) => IV[stat];

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetIV(FMainStatHandle stat, int value, bool recalculate = true)
    {
        IV[stat] = Math.Clamp(value, 0, MaxIV);
        if (recalculate)
            RecalculateStats();
    }

    [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Get IV Override", Category = "Stats")]
    public Option<int> GetIVOverride(FMainStatHandle stat)
    {
        return IVOverrides.TryGetValue(stat, out var value) ? value : Option<int>.None;
    }

    [UFunction(
        FunctionFlags.BlueprintCallable,
        DisplayName = "Set IV Override",
        Category = "Stats"
    )]
    public void SetIVOverride(FMainStatHandle stat, int value, bool recalculate = true)
    {
        IVOverrides.Add(stat, value);
        if (recalculate)
            RecalculateStats();
    }

    [UFunction(
        FunctionFlags.BlueprintCallable,
        DisplayName = "Clear IV Override",
        Category = "Stats"
    )]
    public void ClearIVOverride(FMainStatHandle stat, bool recalculate = true)
    {
        IVOverrides.Remove(stat);
        if (recalculate)
            RecalculateStats();
    }

    [UFunction(FunctionFlags.BlueprintPure, Category = "Stats")]
    public int GetEV(FMainStatHandle stat) => EV[stat];

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void SetEV(FMainStatHandle stat, int value, bool recalculate = true)
    {
        EV[stat] = Math.Clamp(value, 0, MaxEV);
        if (recalculate)
            RecalculateStats();
    }

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

    [ExcludeFromExtensions]
    public void Heal()
    {
        HP = MaxHP;
    }
}
