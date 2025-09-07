using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using GameDataAccessTools.Core.Utilities;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.Model.HardCoded;

[UEnum]
public enum EPokemonGender : byte
{
    Male = 0,
    Female = 1,
    Genderless = 2,
}

[UEnum]
public enum ESpecialGenderRatio : byte
{
    None,
    MaleOnly,
    FemaleOnly,
    GenderlessOnly,
}

[UStruct]
public readonly partial struct FGenderRatioData
{
    [UProperty]
    private readonly ESpecialGenderRatio _specialGenderRatio;

    [UProperty]
    private readonly byte _femaleChance;

    private FGenderRatioData(EPokemonGender gender)
    {
        _specialGenderRatio = gender switch
        {
            EPokemonGender.Male => ESpecialGenderRatio.MaleOnly,
            EPokemonGender.Female => ESpecialGenderRatio.FemaleOnly,
            EPokemonGender.Genderless => ESpecialGenderRatio.GenderlessOnly,
            _ => throw new ArgumentOutOfRangeException(nameof(gender), gender, null),
        };
    }

    private FGenderRatioData(byte femaleChance)
    {
        _specialGenderRatio = ESpecialGenderRatio.None;
        _femaleChance = femaleChance;
    }

    public static FGenderRatioData SingleGender(EPokemonGender gender) => new(gender);

    public static FGenderRatioData FemaleChance(byte chance) => new(chance);

    public static implicit operator FGenderRatioData(EPokemonGender gender) => SingleGender(gender);

    public bool IsSingleGender => _specialGenderRatio != ESpecialGenderRatio.None;

    public void Match(Action<byte> onFemaleChance, Action<EPokemonGender> onSingleGender)
    {
        switch (_specialGenderRatio)
        {
            case ESpecialGenderRatio.None:
                onFemaleChance(_femaleChance);
                break;
            case ESpecialGenderRatio.MaleOnly:
                onSingleGender(EPokemonGender.Male);
                break;
            case ESpecialGenderRatio.FemaleOnly:
                onSingleGender(EPokemonGender.Female);
                break;
            case ESpecialGenderRatio.GenderlessOnly:
                onSingleGender(EPokemonGender.Genderless);
                break;
            default:
                throw new InvalidOperationException();
        }
    }

    public T Match<T>(Func<byte, T> onFemaleChance, Func<EPokemonGender, T> onSingleGender)
    {
        return _specialGenderRatio switch
        {
            ESpecialGenderRatio.None => onFemaleChance(_femaleChance),
            ESpecialGenderRatio.MaleOnly => onSingleGender(EPokemonGender.Male),
            ESpecialGenderRatio.FemaleOnly => onSingleGender(EPokemonGender.Female),
            ESpecialGenderRatio.GenderlessOnly => onSingleGender(EPokemonGender.Genderless),
            _ => throw new InvalidOperationException(),
        };
    }
}

[UStruct]
[CreateStructView]
public readonly partial struct FGenderRatio : IGameDataEntry
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

    [field: UProperty(PropertyFlags.BlueprintReadOnly)]
    [AsValue]
    [UsedImplicitly]
    public required FGenderRatioData Ratio { get; init; }
}

[UClass]
[GameDataRepository<FGenderRatio>]
[UsedImplicitly]
public partial class UGenderRatioRepository : UStaticGameDataRepository;

[UStruct]
[DataHandle(typeof(GameData), nameof(GameData.GenderRatios))]
public readonly partial record struct FGenderRatioHandle;

public static class GenderRatioExtensions
{
    private const string LocalizationNamespace = "GameData.GenderRatio";

    public static UGameDataManager AddGenderRatios(this UGameDataManager manager)
    {
        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.AlwaysMale,
                Name = FText.Localized(LocalizationNamespace, "AlwaysMale", "Always Male"),
                Ratio = EPokemonGender.Male,
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.AlwaysFemale,
                Name = FText.Localized(LocalizationNamespace, "AlwaysFemale", "Always Female"),
                Ratio = EPokemonGender.Female,
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Genderless,
                Name = FText.Localized(LocalizationNamespace, "Genderless", "Genderless"),
                Ratio = EPokemonGender.Genderless,
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.FemaleOneEighth,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "FemaleOneEighth",
                    "Female One Eighth"
                ),
                Ratio = FGenderRatioData.FemaleChance(32),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female25Percent,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "Female25Percent",
                    "Female 25 Percent"
                ),
                Ratio = FGenderRatioData.FemaleChance(64),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female50Percent,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "Female50Percent",
                    "Female 50 Percent"
                ),
                Ratio = FGenderRatioData.FemaleChance(128),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female75Percent,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "Female75Percent",
                    "Female 75 Percent"
                ),
                Ratio = FGenderRatioData.FemaleChance(192),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.FemaleSevenEighths,
                Name = FText.Localized(
                    LocalizationNamespace,
                    "FemaleSevenEighths",
                    "Female Seven Eighths"
                ),
                Ratio = FGenderRatioData.FemaleChance(224),
            }
        );

        return manager;
    }
}
