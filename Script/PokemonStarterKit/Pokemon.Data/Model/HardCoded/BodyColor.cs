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
public readonly partial struct FBodyColor : IGameDataEntry
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
[GameDataRepository<FBodyColor>]
[UsedImplicitly]
public partial class UBodyColorRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BodyColors))]
public readonly partial record struct FBodyColorHandle;

public static class BodyColorExtensions
{
    private const string LocalizationNamespace = "GameData.BodyColor";

    public static UGameDataManager AddBodyColors(this UGameDataManager manager)
    {
        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Red,
                Name = FText.Localized(LocalizationNamespace, "Red", "Red"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Blue,
                Name = FText.Localized(LocalizationNamespace, "Blue", "Blue"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Yellow,
                Name = FText.Localized(LocalizationNamespace, "Yellow", "Yellow"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Green,
                Name = FText.Localized(LocalizationNamespace, "Green", "Green"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Black,
                Name = FText.Localized(LocalizationNamespace, "Black", "Black"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Brown,
                Name = FText.Localized(LocalizationNamespace, "Brown", "Brown"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Purple,
                Name = FText.Localized(LocalizationNamespace, "Purple", "Purple"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Gray,
                Name = FText.Localized(LocalizationNamespace, "Gray", "Gray"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.White,
                Name = FText.Localized(LocalizationNamespace, "White", "White"),
            }
        );

        manager.BodyColors.RegisterEntry(
            new FBodyColor
            {
                ID = FBodyColor.Pink,
                Name = FText.Localized(LocalizationNamespace, "Pink", "Pink"),
            }
        );

        return manager;
    }
}
