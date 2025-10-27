using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

/// <summary>
/// Represents the terrain configuration used in battles.
/// Provides unique identification, localization, and other essential properties for terrain types.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FBattleTerrain : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the localized name of the battle terrain.
    /// This property is required and provides a human-readable name for the terrain type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }
}

/// <summary>
/// Serves as a repository for managing and accessing instances of FBattleTerrain.
/// Extends the UStaticGameDataRepository to facilitate terrain-specific game data operations.
/// </summary>
[UClass]
[GameDataRepository<FBattleTerrain>]
public partial class UBattleTerrainRepository : UStaticGameDataRepository;

/// <summary>
/// A handle representing a reference to a BattleTerrain entry within the game's data repository.
/// Provides an identifier for terrain-related data, enabling efficient access and retrieval from the associated repository.
/// </summary>
[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.BattleTerrains))]
public readonly partial record struct FBattleTerrainHandle([property: UProperty(PropertyFlags.EditAnywhere)] FName ID);

/// <summary>
/// Provides extension methods for managing and registering battle terrain data
/// within the game's data management framework.
/// </summary>
public static class BattleTerrainExtensions
{
    private const string LocalizationNamespace = "GameData.BattleTerrain";

    /// <summary>
    /// Registers a set of predefined battle terrains into the game data manager.
    /// </summary>
    /// <param name="manager">
    /// The instance of <see cref="UGameDataManager"/> to which the battle terrains will be added.
    /// </param>
    /// <returns>
    /// The updated instance of <see cref="UGameDataManager"/> containing the newly added battle terrains.
    /// </returns>
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
