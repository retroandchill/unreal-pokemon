using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UStruct]
public readonly partial record struct FNatureStatChange(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FMainBattleStatHandle StatHandle,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int PercentChange
);

[UStruct]
[CreateStructView]
public readonly partial struct FNature() : IGameDataEntry
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
    public IReadOnlyList<FNatureStatChange> StatChanges { get; init; } = [];
}

[UClass]
[GameDataRepository<FNature>]
[UsedImplicitly]
public partial class UNatureRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.Natures))]
public readonly partial record struct FNatureHandle;

public static class NatureExtensions
{
    private const string LocalizationNamespace = "GameData.Nature";

    public static UGameDataManager AddNatures(this UGameDataManager manager)
    {
        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.HARDY,
                Name = FText.Localized(LocalizationNamespace, "HARDY", "Hardy"),
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.LONELY,
                Name = FText.Localized(LocalizationNamespace, "LONELY", "Lonely"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.BRAVE,
                Name = FText.Localized(LocalizationNamespace, "BRAVE", "Brave"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.ADAMANT,
                Name = FText.Localized(LocalizationNamespace, "ADAMANT", "Adamant"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.NAUGHTY,
                Name = FText.Localized(LocalizationNamespace, "NAUGHTY", "Naughty"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.BOLD,
                Name = FText.Localized(LocalizationNamespace, "BOLD", "Bold"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.DOCILE,
                Name = FText.Localized(LocalizationNamespace, "DOCILE", "Docile"),
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.RELAXED,
                Name = FText.Localized(LocalizationNamespace, "RELAXED", "Relaxed"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.IMPISH,
                Name = FText.Localized(LocalizationNamespace, "IMPISH", "Impish"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.LAX,
                Name = FText.Localized(LocalizationNamespace, "LAX", "Lax"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.TIMID,
                Name = FText.Localized(LocalizationNamespace, "TIMID", "Timid"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.HASTY,
                Name = FText.Localized(LocalizationNamespace, "HASTY", "Hasty"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.SERIOUS,
                Name = FText.Localized(LocalizationNamespace, "SERIOUS", "Serious"),
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.JOLLY,
                Name = FText.Localized(LocalizationNamespace, "JOLLY", "Jolly"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.NAIVE,
                Name = FText.Localized(LocalizationNamespace, "NAIVE", "Naive"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPEED, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.MODEST,
                Name = FText.Localized(LocalizationNamespace, "MODEST", "Modest"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.MILD,
                Name = FText.Localized(LocalizationNamespace, "MILD", "Mild"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.QUIET,
                Name = FText.Localized(LocalizationNamespace, "QUIET", "Quiet"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.BASHFUL,
                Name = FText.Localized(LocalizationNamespace, "BASHFUL", "Bashful"),
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.RASH,
                Name = FText.Localized(LocalizationNamespace, "RASH", "Rash"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, 10),
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.CALM,
                Name = FText.Localized(LocalizationNamespace, "CALM", "Calm"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.GENTLE,
                Name = FText.Localized(LocalizationNamespace, "GENTLE", "Gentle"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.DEFENSE, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.SASSY,
                Name = FText.Localized(LocalizationNamespace, "SASSY", "Sassy"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.SPEED, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.CAREFUL,
                Name = FText.Localized(LocalizationNamespace, "CAREFUL", "Careful"),
                StatChanges =
                [
                    new FNatureStatChange(FStat.SPECIAL_DEFENSE, 10),
                    new FNatureStatChange(FStat.SPECIAL_ATTACK, -10),
                ],
            }
        );

        manager.Natures.RegisterEntry(
            new FNature
            {
                ID = FNature.QUIRKY,
                Name = FText.Localized(LocalizationNamespace, "QUIRKY", "Quirky"),
            }
        );

        return manager;
    }
}
