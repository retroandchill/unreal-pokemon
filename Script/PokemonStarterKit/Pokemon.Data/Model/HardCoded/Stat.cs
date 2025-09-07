using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using GameDataAccessTools.Core.Views;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;
using ZLinq;
using ZLinq.Linq;

namespace Pokemon.Data.Model.HardCoded;

using FilteredStats = ValueEnumerable<
    WhereSelect<
        StructArrayValueEnumerator<FStat, (FName ID, FText Name, EStatType StatType)>,
        (FName ID, FText Name, EStatType StatType),
        FDataHandleEntry
    >,
    FDataHandleEntry
>;

[UEnum]
public enum EStatType : byte
{
    Main,
    MainBattle,
    Battle,
}

[UStruct]
[CreateStructView]
public readonly partial struct FStat : IGameDataEntry
{
    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FText NameBrief { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    public required EStatType StatType { get; init; }

    [UsedImplicitly]
    [field: UProperty(PropertyFlags.BlueprintReadOnly, DisplayName = "PBS Order")]
    public int PBSOrder { get; init; }
}

[UClass]
[GameDataRepository<FStat>]
[UsedImplicitly]
public partial class UStatRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(
    typeof(GameData),
    nameof(GameData.Stats),
    ComparableTypes = [
        typeof(FMainStatHandle),
        typeof(FBattleStatHandle),
        typeof(FMainBattleStatHandle),
    ]
)]
public readonly partial record struct FStatHandle;

[UStruct]
[DataHandle<FStat>(
    ComparableTypes = [
        typeof(FStatHandle),
        typeof(FBattleStatHandle),
        typeof(FMainBattleStatHandle),
    ]
)]
public readonly partial record struct FMainStatHandle
{
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType is EStatType.Main or EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType is EStatType.Main or EStatType.MainBattle;
        }
    }

    public StructView<FStat> Entry
    {
        get
        {
            if (
                !GameData.Stats.TryGetEntry(ID, out var entry)
                || entry.StatType is not EStatType.Main and not EStatType.MainBattle
            )
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

[UStruct]
[DataHandle<FStat>(
    ComparableTypes = [typeof(FStatHandle), typeof(FMainStatHandle), typeof(FMainBattleStatHandle)]
)]
public readonly partial record struct FBattleStatHandle
{
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType is EStatType.Battle or EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType is EStatType.Battle or EStatType.MainBattle;
        }
    }

    public StructView<FStat> Entry
    {
        get
        {
            if (
                !GameData.Stats.TryGetEntry(ID, out var entry)
                || entry.StatType is not EStatType.Battle and not EStatType.MainBattle
            )
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

[UStruct]
[DataHandle<FStat>(
    ComparableTypes = [typeof(FStatHandle), typeof(FBattleStatHandle), typeof(FMainStatHandle)]
)]
public readonly partial record struct FMainBattleStatHandle
{
    public static FilteredStats Entries
    {
        get
        {
            return GameData
                .Stats.Entries.AsValueEnumerable(s => (s.ID, s.Name, s.StatType))
                .Where(s => s.StatType == EStatType.MainBattle)
                .Select(s => new FDataHandleEntry(s.ID, s.Name));
        }
    }

    static IEnumerable<FDataHandleEntry> IDataHandle.Entries => Entries.ToArray();

    public bool IsValid
    {
        get
        {
            if (!GameData.Stats.TryGetEntry(ID, out var entry))
            {
                return false;
            }

            return entry.StatType == EStatType.MainBattle;
        }
    }

    public StructView<FStat> Entry
    {
        get
        {
            if (
                !GameData.Stats.TryGetEntry(ID, out var entry)
                || entry.StatType != EStatType.MainBattle
            )
            {
                throw new InvalidOperationException("Invalid stat handle!");
            }

            return entry;
        }
    }
}

public static class StatExtensions
{
    private const string LocalizationNamespace = "GameData.Stat";

    public static UGameDataManager AddStats(this UGameDataManager manager)
    {
        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.HP,
                Name = FText.Localized(LocalizationNamespace, "HP", "HP"),
                NameBrief = FText.Localized(LocalizationNamespace, "HP", "HP"),
                StatType = EStatType.Main,
                PBSOrder = 0,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.ATTACK,
                Name = FText.Localized(LocalizationNamespace, "Attack", "Attack"),
                NameBrief = FText.Localized(LocalizationNamespace, "Atk", "Atk"),
                StatType = EStatType.MainBattle,
                PBSOrder = 1,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.DEFENSE,
                Name = FText.Localized(LocalizationNamespace, "Defense", "Defense"),
                NameBrief = FText.Localized(LocalizationNamespace, "Def", "Def"),
                StatType = EStatType.MainBattle,
                PBSOrder = 2,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPECIAL_ATTACK,
                Name = FText.Localized(LocalizationNamespace, "SpecialAttack", "SpecialAttack"),
                NameBrief = FText.Localized(LocalizationNamespace, "SpAtk", "SpAtk"),
                StatType = EStatType.MainBattle,
                PBSOrder = 4,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPECIAL_DEFENSE,
                Name = FText.Localized(LocalizationNamespace, "SpecialDefense", "SpecialDefense"),
                NameBrief = FText.Localized(LocalizationNamespace, "SpDef", "SpDef"),
                StatType = EStatType.MainBattle,
                PBSOrder = 5,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.SPEED,
                Name = FText.Localized(LocalizationNamespace, "Speed", "Speed"),
                NameBrief = FText.Localized(LocalizationNamespace, "Spd", "Spd"),
                StatType = EStatType.MainBattle,
                PBSOrder = 3,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.ACCURACY,
                Name = FText.Localized(LocalizationNamespace, "accuracy", "accuracy"),
                NameBrief = FText.Localized(LocalizationNamespace, "Acc", "Acc"),
                StatType = EStatType.Battle,
            }
        );

        manager.Stats.RegisterEntry(
            new FStat
            {
                ID = FStat.EVASION,
                Name = FText.Localized(LocalizationNamespace, "evasiveness", "evasiveness"),
                NameBrief = FText.Localized(LocalizationNamespace, "Eva", "Eva"),
                StatType = EStatType.Battle,
            }
        );

        return manager;
    }
}
