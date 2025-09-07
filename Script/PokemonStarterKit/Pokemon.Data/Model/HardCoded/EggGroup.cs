using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UEnum]
public enum EEggGroupType : byte
{
    WithSameEggGroup,
    WithAnyEggGroup,
    WithNoEggGroups,
}

[UStruct]
[CreateStructView]
public readonly partial struct FEggGroup : IGameDataEntry
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
    public EEggGroupType BreedingType { get; init; }
}

[UClass]
[GameDataRepository<FEggGroup>]
[UsedImplicitly]
public partial class UEggGroupRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.EggGroups))]
public readonly partial record struct FEggGroupHandle;

public static class EggGroupExtensions
{
    private const string LocalizationNamespace = "GameData.EggGroup";

    public static UGameDataManager AddEggGroups(this UGameDataManager manager)
    {
        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Undiscovered,
                Name = FText.Localized(LocalizationNamespace, "Undiscovered", "Undiscovered"),
                BreedingType = EEggGroupType.WithNoEggGroups,
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Monster,
                Name = FText.Localized(LocalizationNamespace, "Monster", "Monster"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Water1,
                Name = FText.Localized(LocalizationNamespace, "Water1", "Water 1"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Bug,
                Name = FText.Localized(LocalizationNamespace, "Bug", "Bug"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Flying,
                Name = FText.Localized(LocalizationNamespace, "Flying", "Flying"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Field,
                Name = FText.Localized(LocalizationNamespace, "Field", "Field"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Fairy,
                Name = FText.Localized(LocalizationNamespace, "Fairy", "Fairy"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Grass,
                Name = FText.Localized(LocalizationNamespace, "Grass", "Grass"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Humanlike,
                Name = FText.Localized(LocalizationNamespace, "Humanlike", "Humanlike"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Water3,
                Name = FText.Localized(LocalizationNamespace, "Water3", "Water 3"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Mineral,
                Name = FText.Localized(LocalizationNamespace, "Mineral", "Mineral"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Amorphous,
                Name = FText.Localized(LocalizationNamespace, "Amorphous", "Amorphous"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Water2,
                Name = FText.Localized(LocalizationNamespace, "Water2", "Water 2"),
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Ditto,
                Name = FText.Localized(LocalizationNamespace, "Ditto", "Ditto"),
                BreedingType = EEggGroupType.WithAnyEggGroup,
            }
        );

        manager.EggGroups.RegisterEntry(
            new FEggGroup
            {
                ID = FEggGroup.Dragon,
                Name = FText.Localized(LocalizationNamespace, "Dragon", "Dragon"),
            }
        );

        return manager;
    }
}
