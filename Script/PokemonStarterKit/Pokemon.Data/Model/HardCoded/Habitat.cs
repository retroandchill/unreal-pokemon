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
public readonly partial struct FHabitat : IGameDataEntry
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
[GameDataRepository<FHabitat>]
[UsedImplicitly]
public partial class UHabitatRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Habitats))]
public readonly partial record struct FHabitatHandle;

public static class HabitatExtensions
{
    private const string LocalizationNamespace = "GameData.Habitat";

    public static UGameDataManager AddHabitats(this UGameDataManager manager)
    {
        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Grassland,
                Name = FText.Localized(LocalizationNamespace, "Grassland", "Grassland"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Forest,
                Name = FText.Localized(LocalizationNamespace, "Forest", "Forest"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.WatersEdge,
                Name = FText.Localized(LocalizationNamespace, "WatersEdge", "Water's Edge"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Sea,
                Name = FText.Localized(LocalizationNamespace, "Sea", "Sea"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Cave,
                Name = FText.Localized(LocalizationNamespace, "Cave", "Cave"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Mountain,
                Name = FText.Localized(LocalizationNamespace, "Mountain", "Mountain"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.RoughTerrain,
                Name = FText.Localized(LocalizationNamespace, "RoughTerrain", "Rough Terrain"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Urban,
                Name = FText.Localized(LocalizationNamespace, "Urban", "Urban"),
            }
        );

        manager.Habitats.RegisterEntry(
            new FHabitat
            {
                ID = FHabitat.Rare,
                Name = FText.Localized(LocalizationNamespace, "Rare", "Rare"),
            }
        );

        return manager;
    }
}
