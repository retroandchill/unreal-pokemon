using GameAccessTools.SourceGenerator.Attributes;
using GameDataAccessTools.Core;
using JetBrains.Annotations;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameDataAccessTools;

namespace Pokemon.Data.HardCoded;

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

public static class GenderRatioExtensions { }
