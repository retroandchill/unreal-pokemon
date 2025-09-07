using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
[CreateStructView]
public readonly partial struct FStatus : IGameDataEntry
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
}

[UClass]
[GameDataRepository<FStatus>]
[UsedImplicitly]
public partial class UStatusRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Statuses))]
public readonly partial record struct FStatusHandle;

public static class StatusExtensions
{
    private const string LocalizationNamespace = "GameData.Status";

    public static UGameDataManager AddStatuses(this UGameDataManager manager)
    {
        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.SLEEP,
                Name = FText.Localized(LocalizationNamespace, "Sleep", "Sleep"),
            }
        );

        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.POISON,
                Name = FText.Localized(LocalizationNamespace, "Poison", "Poison"),
            }
        );

        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.BURN,
                Name = FText.Localized(LocalizationNamespace, "Burn", "Burn"),
            }
        );

        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.PARALYSIS,
                Name = FText.Localized(LocalizationNamespace, "Paralysis", "Paralysis"),
            }
        );

        manager.Statuses.RegisterEntry(
            new FStatus
            {
                ID = FStatus.FROZEN,
                Name = FText.Localized(LocalizationNamespace, "Frozen", "Frozen"),
            }
        );

        return manager;
    }
}
