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
/// Represents the gender of a Pokémon.
/// </summary>
/// <remarks>
/// The <see cref="EPokemonGender"/> enumeration is used to categorize the gender of a Pokémon.
/// It contains three possible values: Male, Female, and Genderless. Genders influence gameplay
/// mechanics such as breeding, compatibility, and form availability for specific Pokémon species.
/// Some Pokémon are bound to specific genders due to their unique characteristics, while others
/// can also be genderless.
/// </remarks>
[UEnum]
public enum EPokemonGender : byte
{
    /// <summary>
    /// Male gender.
    /// </summary>
    Male = 0,

    /// <summary>
    /// Female gender.
    /// </summary>
    Female = 1,

    /// <summary>
    /// Gender unknown.
    /// </summary>
    Genderless = 2,
}

/// <summary>
/// Represents special gender ratios for Pokémon species.
/// </summary>
/// <remarks>
/// The <see cref="ESpecialGenderRatio"/> enumeration is used to define specific gender constraints
/// for Pokémon species. It helps categorize species that deviate from standard male-to-female ratios or
/// exhibit unique gender characteristics. These constraints can specify whether a species is exclusively
/// male, exclusively female, entirely genderless, or does not fall under any special designation.
/// This information is essential for gameplay mechanics including breeding and encounter logic.
/// </remarks>
[UEnum]
public enum ESpecialGenderRatio : byte
{
    /// <summary>
    /// No special gender ratio.
    /// </summary>
    None,

    /// <summary>
    /// All Pokémon are exclusively male.
    /// </summary>
    MaleOnly,

    /// <summary>
    /// All Pokémon are exclusively female.
    /// </summary>
    FemaleOnly,

    /// <summary>
    /// All Pokémon are genderless.
    /// </summary>
    GenderlessOnly,
}

/// <summary>
/// Represents gender ratio data for Pokémon. This struct encapsulates the gender-related
/// properties and behaviors, such as single-gender constraints or female percentage chances,
/// that can be associated with Pokémon entities.
/// </summary>
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

    /// <summary>
    /// Creates a gender ratio data instance representing a single-gender Pokémon.
    /// </summary>
    /// <param name="gender">The gender of the Pokémon. Can be Male, Female, or Genderless as specified by the EPokemonGender enumeration.</param>
    /// <returns>A gender ratio data instance for the specified single gender.</returns>
    public static FGenderRatioData SingleGender(EPokemonGender gender) => new(gender);

    /// <summary>
    /// Creates a gender ratio data instance representing a specific chance for the Pokémon to be female.
    /// </summary>
    /// <param name="chance">The percentage chance for the Pokémon to be female, represented as a byte in the range of 0 to 255.</param>
    /// <returns>A gender ratio data instance with the specified female chance.</returns>
    public static FGenderRatioData FemaleChance(byte chance) => new(chance);

    /// <summary>
    /// Implicitly converts an <see cref="EPokemonGender"/> value to an instance of <see cref="FGenderRatioData"/>.
    /// </summary>
    /// <param name="gender">The gender of the Pokémon to convert. Must be a value from the <see cref="EPokemonGender"/> enumeration.</param>
    /// <returns>
    /// A new <see cref="FGenderRatioData"/> instance representing the specified single gender.
    /// </returns>
    public static implicit operator FGenderRatioData(EPokemonGender gender) => SingleGender(gender);

    /// <summary>
    /// Indicates whether the associated entity is confined to a single gender.
    /// This property evaluates the gender ratio data and determines if the entity
    /// exclusively represents one gender (e.g., male-only, female-only, or genderless-only).
    /// </summary>
    public bool IsSingleGender => _specialGenderRatio != ESpecialGenderRatio.None;

    /// <summary>
    /// Applies specific operations based on the gender ratio data type.
    /// </summary>
    /// <param name="onFemaleChance">The action to execute if the gender ratio data represents a female chance, providing the chance as a byte parameter.</param>
    /// <param name="onSingleGender">The action to execute if the gender ratio data represents a single-gender Pokémon, providing the gender as an EPokemonGender parameter.</param>
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

    /// <summary>
    /// Matches the gender ratio data with the specified logic for female chance or single-gender scenarios.
    /// </summary>
    /// <param name="onFemaleChance">A function to handle cases where the gender is determined by a female chance value.</param>
    /// <param name="onSingleGender">A function to handle cases where the Pokémon has a single-gender specification.</param>
    /// <returns>The result of executing the matching logic based on the provided functions.</returns>
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

/// <summary>
/// Represents the gender ratio configuration for a Pokémon. This struct defines
/// key attributes related to gender such as its unique identifier, display name,
/// and the gender ratio data encapsulated by an associated data structure.
/// </summary>
[UStruct]
[CreateStructView]
public readonly partial struct FGenderRatio : IGameDataEntry
{
    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public required FName ID { get; init; }

    /// <inheritdoc />
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    public int RowIndex { get; init; }

    /// <summary>
    /// Represents the display name associated with a gender ratio entry.
    /// This property provides a localized text value, typically used to describe
    /// the gender distribution of an entity in a user-readable format.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [DisplayName]
    public required FText Name { get; init; }

    /// <summary>
    /// Represents the gender ratio data associated with an entity.
    /// This property provides information regarding the distribution
    /// of genders for the specified entity.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly)]
    [AsValue]
    public required FGenderRatioData Ratio { get; init; }
}

/// <summary>
/// Represents a repository class for managing and accessing gender ratio data in the Pokémon game.
/// This repository specializes in handling data operations associated with the <see cref="FGenderRatio"/> structure.
/// </summary>
[UClass]
[GameDataRepository<FGenderRatio>]
public partial class UGenderRatioRepository : UStaticGameDataRepository;

/// <summary>
/// Represents a reference or handle to gender ratio data within the Pokémon game data.
/// This handle serves as an intermediary for accessing specific gender-based configurations or distributions
/// for Pokémon entities, as defined in the game's data repository.
/// </summary>
[DataHandle(typeof(GameData), nameof(GameData.GenderRatios))]
public readonly partial record struct FGenderRatioHandle;

/// <summary>
/// Provides extension methods for managing and registering gender ratio data
/// associated with Pokémon. This static class simplifies the addition of predefined
/// gender ratios to a given <c>UGameDataManager</c> instance, allowing for streamlined
/// manipulation and configuration of gender-related properties in the Pokémon dataset.
/// </summary>
public static class GenderRatioExtensions
{
    private const string LocalizationNamespace = "GameData.GenderRatio";

    /// <summary>
    /// Adds predefined gender ratio data entries to the game data manager.
    /// </summary>
    /// <param name="manager">The instance of <c>UGameDataManager</c> to which the gender ratios are added.</param>
    /// <returns>The <c>UGameDataManager</c> instance with the gender ratios added.</returns>
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
                Name = FText.Localized(LocalizationNamespace, "FemaleOneEighth", "Female One Eighth"),
                Ratio = FGenderRatioData.FemaleChance(32),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female25Percent,
                Name = FText.Localized(LocalizationNamespace, "Female25Percent", "Female 25 Percent"),
                Ratio = FGenderRatioData.FemaleChance(64),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female50Percent,
                Name = FText.Localized(LocalizationNamespace, "Female50Percent", "Female 50 Percent"),
                Ratio = FGenderRatioData.FemaleChance(128),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.Female75Percent,
                Name = FText.Localized(LocalizationNamespace, "Female75Percent", "Female 75 Percent"),
                Ratio = FGenderRatioData.FemaleChance(192),
            }
        );

        manager.GenderRatios.RegisterEntry(
            new FGenderRatio
            {
                ID = FGenderRatio.FemaleSevenEighths,
                Name = FText.Localized(LocalizationNamespace, "FemaleSevenEighths", "Female Seven Eighths"),
                Ratio = FGenderRatioData.FemaleChance(224),
            }
        );

        return manager;
    }
}
