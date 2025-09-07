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
public readonly partial struct FBodyShape : IGameDataEntry
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
[GameDataRepository<FBodyShape>]
[UsedImplicitly]
public partial class UBodyShapeRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BodyShapes))]
public readonly partial record struct FBodyShapeHandle;

public static class BodyShapeExtensions
{
    private const string LocalizationNamespace = "GameData.BodyShape";

    public static UGameDataManager AddBodyShapes(this UGameDataManager manager)
    {
        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Head,
                Name = FText.Localized(LocalizationNamespace, "Head", "Head"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Serpentine,
                Name = FText.Localized(LocalizationNamespace, "Serpentine", "Serpentine"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Finned,
                Name = FText.Localized(LocalizationNamespace, "Finned", "Finned"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadArms,
                Name = FText.Localized(LocalizationNamespace, "HeadArms", "Head and arms"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadBase,
                Name = FText.Localized(LocalizationNamespace, "HeadBase", "Head and base"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.BipedalTail,
                Name = FText.Localized(LocalizationNamespace, "BipedalTail", "Bipedal with tail"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.HeadLegs,
                Name = FText.Localized(LocalizationNamespace, "HeadLegs", "Head and legs"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Quadruped,
                Name = FText.Localized(LocalizationNamespace, "Quadruped", "Quadruped"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Winged,
                Name = FText.Localized(LocalizationNamespace, "Winged", "Winged"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Multiped,
                Name = FText.Localized(LocalizationNamespace, "Multiped", "Multiped"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.MultiBody,
                Name = FText.Localized(LocalizationNamespace, "MultiBody", "Multi Body"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Bipedal,
                Name = FText.Localized(LocalizationNamespace, "Bipedal", "Bipedal"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.MultiWinged,
                Name = FText.Localized(LocalizationNamespace, "MultiWinged", "Multi Winged"),
            }
        );

        manager.BodyShapes.RegisterEntry(
            new FBodyShape
            {
                ID = FBodyShape.Insectoid,
                Name = FText.Localized(LocalizationNamespace, "Insectoid", "Insectoid"),
            }
        );

        return manager;
    }
}
