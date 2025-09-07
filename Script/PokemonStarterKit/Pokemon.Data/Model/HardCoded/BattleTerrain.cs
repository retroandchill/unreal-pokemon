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
public readonly partial struct FBattleTerrain : IGameDataEntry
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
[GameDataRepository<FBattleTerrain>]
[UsedImplicitly]
public partial class UBattleTerrainRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BattleTerrains))]
public readonly partial record struct FBattleTerrainHandle;

public static class BattleTerrainExtensions
{
    private const string LocalizationNamespace = "GameData.BattleTerrain";

    public static UGameDataManager AddBattleTerrains(this UGameDataManager manager)
    {
        manager.BattleTerrains.RegisterEntry(
            new FBattleTerrain
            {
                ID = FBattleTerrain.Electric,
                Name = FText.Localized(LocalizationNamespace, "Electric", "Electric"),
            }
        );

        manager.BattleTerrains.RegisterEntry(
            new FBattleTerrain
            {
                ID = FBattleTerrain.Grassy,
                Name = FText.Localized(LocalizationNamespace, "Grassy", "Grassy"),
            }
        );

        manager.BattleTerrains.RegisterEntry(
            new FBattleTerrain
            {
                ID = FBattleTerrain.Misty,
                Name = FText.Localized(LocalizationNamespace, "Misty", "Misty"),
            }
        );

        manager.BattleTerrains.RegisterEntry(
            new FBattleTerrain
            {
                ID = FBattleTerrain.Psychic,
                Name = FText.Localized(LocalizationNamespace, "Psychic", "Psychic"),
            }
        );

        return manager;
    }
}
