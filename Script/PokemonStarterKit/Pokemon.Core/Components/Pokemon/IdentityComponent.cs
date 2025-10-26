using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Provides identity-related attributes and behaviors for a Pokémon entity.
/// This component tracks various characteristics of a Pokémon, such as its species, nickname, gender,
/// personality, and shininess status.
/// </summary>
[UClass]
[UMetaData("HideCategories", "Identity")]
public partial class UIdentityComponent : URPGComponent
{
    /// <summary>
    /// Represents the species associated with a Pokémon.
    /// This property defines the specific species identity of the Pokémon, including species-related data such as
    /// its name, type(s), and gender ratio. It plays a central role in determining numerous characteristics of
    /// the Pokémon and its behavior within the game context.
    /// </summary>
    /// <remarks>
    /// The species data is linked with the Pokémon's base attributes and dictates core gameplay behaviors, such as ability to evolve, types, and availability.
    /// </remarks>
    /// <value>
    /// A handle to the species data representing the Pokémon's species identity.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public partial FSpeciesHandle Species { get; set; }

    /// <summary>
    /// Represents the custom name assigned to a Pokémon.
    /// This property allows for personalization by enabling the assignment of a unique name to distinguish a Pokémon from others of the same species.
    /// </summary>
    /// <remarks>
    /// A Pokémon's nickname is set and modified by the user or system after initialization.
    /// While the nickname may differ from the species name, it has no impact on the Pokémon's abilities, type, or other gameplay mechanics.
    /// </remarks>
    /// <value>
    /// A localized text object containing the assigned nickname of the Pokémon.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public partial FText Nickname { get; set; }

    /// <summary>
    /// Represents the personality value of a Pokémon.
    /// This property is a unique identifier influencing various aspects of a Pokémon's traits, behavior, and interactions.
    /// It is used in multiple mechanics, including gender determination, ability assignment, nature selection, and shiny status computation.
    /// </summary>
    /// <remarks>
    /// The personality value is a pseudorandom 32-bit unsigned integer generated during a Pokémon's initialization.
    /// It serves as a deterministic factor in multiple gameplay systems, enhancing unpredictability and individuality of each Pokémon.
    /// </remarks>
    /// <value>
    /// A 32-bit unsigned integer representing the personality value of the Pokémon.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public partial uint PersonalityValue { get; set; }

    /// <summary>
    /// Represents the gender of a Pokémon.
    /// This property determines the gender identity of a Pokémon, which can affect its behavior,
    /// breeding compatibility, and in-game interactions. The gender is determined based on the
    /// species' gender ratio, or it may be explicitly defined for certain Pokémon that are
    /// single-gendered or genderless.
    /// </summary>
    /// <remarks>
    /// The gender property is computed dynamically for Pokémon with variable gender ratios based on
    /// their personality value. For species that are single-gendered, this property is constant.
    /// Gender influences key gameplay mechanics such as breeding and in some cases evolves or
    /// determines availability of forms.
    /// </remarks>
    /// <value>
    /// A value from the <see cref="EPokemonGender"/> enumeration indicating the Pokémon's gender:
    /// Male, Female, or Genderless.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Gender", BlueprintAccessors = true)]
    public EPokemonGender Gender
    {
        get => Gender_BackingField;
        set
        {
            if (IsSingleGendered)
                return;

            Gender_BackingField = value;
        }
    }

    /// <summary>
    /// Indicates whether the associated Pokémon species is single-gendered.
    /// This property determines if the Pokémon's species is inherently restricted to a single gender
    /// (male, female, or genderless), based on its gender ratio configuration.
    /// </summary>
    /// <remarks>
    /// A Pokémon species is considered single-gendered if its gender ratio explicitly designates only one permissible gender.
    /// This is often determined by unique biological or lore-based characteristics of the species.
    /// </remarks>
    /// <value>
    /// A boolean value where <c>true</c> indicates the species is single-gendered, and <c>false</c> otherwise.
    /// </value>
    public bool IsSingleGendered
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Species.Entry.GenderRatio.Entry.Ratio.IsSingleGender;
    }

    /// <summary>
    /// Indicates whether the Pokémon is male.
    /// This property determines the gender of the Pokémon in the game context
    /// and performs a comparison to check if its gender is set to male.
    /// </summary>
    /// <remarks>
    /// Gender in Pokémon can affect various aspects such as appearance, breeding mechanics, and compatibility with certain moves or items.
    /// The gender is determined based on the <c>EPokemonGender</c> enumeration set by the <c>Gender</c> property.
    /// </remarks>
    /// <value>
    /// A boolean value indicating the gender status. Returns <c>true</c> if the Pokémon's gender is male; otherwise, <c>false</c>.
    /// </value>
    public bool IsMale
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Male;
    }

    /// <summary>
    /// Indicates whether the Pokémon is female.
    /// This property determines the gender of the Pokémon by checking if its gender is explicitly set to female.
    /// Gender influences various gameplay mechanics, including breeding compatibility and certain evolutions.
    /// </summary>
    /// <remarks>
    /// The gender of a Pokémon is defined by the <see cref="EPokemonGender"/> enumeration. A Pokémon can be
    /// male, female, or genderless. This property specifically evaluates whether the Pokémon is female.
    /// </remarks>
    /// <value>
    /// A boolean value indicating if the Pokémon is female. Returns <c>true</c> if the Pokémon's gender is
    /// <see cref="EPokemonGender.Female"/>; otherwise, <c>false</c>.
    /// </value>
    public bool IsFemale
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Female;
    }

    /// <summary>
    /// Indicates whether the Pokémon is genderless.
    /// This property determines if the Pokémon belongs to the genderless category, meaning it does not have a specific male or female gender.
    /// </summary>
    /// <remarks>
    /// Genderless Pokémon are typically those that do not have biological gender distinctions. Their gender is defined as <c>EPokemonGender.Genderless</c>.
    /// This property is useful in situations where gender differences influence gameplay, such as breeding or form-specific attributes.
    /// </remarks>
    /// <value>
    /// <c>true</c> if the Pokémon is genderless; otherwise, <c>false</c>.
    /// </value>
    public bool IsGenderless
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Genderless;
    }

    /// <summary>
    /// Represents the elemental or attribute-based types associated with a Pokémon.
    /// This property provides the Pokémon's type information, which determines its strengths, weaknesses,
    /// and interactions with moves and abilities within the game context.
    /// </summary>
    /// <remarks>
    /// The types of a Pokémon define its interactions in battles, including damage multipliers against other types
    /// and resistance or susceptibility to moves. A Pokémon can have one or two types, as defined by its species.
    /// </remarks>
    /// <value>
    /// A read-only collection of type handles representing the Pokémon's type information.
    /// </value>
    public ReadOnlySpan<FTypeHandle> Types => Species.Entry.Types;

    /// <summary>
    /// Indicates whether the Pokémon is shiny.
    /// This property determines if the Pokémon has an alternate, rare coloration, which is often highly sought after in gameplay.
    /// </summary>
    /// <remarks>
    /// The shiny status of a Pokémon is determined based on specific conditions and probabilities, such as the Pokémon's personality value
    /// and other initialization parameters. A shiny Pokémon retains its shiny status permanently and is a key aspect of its identity.
    /// </remarks>
    /// <value>
    /// A boolean value where <c>true</c> signifies that the Pokémon is shiny and <c>false</c> indicates otherwise.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess")]
    public partial bool IsShiny { get; set; }

    /// <summary>
    /// Indicates whether the Pokémon is considered "Super Shiny."
    /// This property determines if a Pokémon has achieved the rare state of being "Super Shiny,"
    /// a special variant of the shiny condition that is even more uncommon and prestigious.
    /// </summary>
    /// <remarks>
    /// Being "Super Shiny" is a distinct property that overrides standard shininess,
    /// automatically setting the Pokémon to shiny status. It signifies a unique
    /// visual and gameplay distinction for the Pokémon.
    /// </remarks>
    /// <value>
    /// A boolean value where true signifies the Pokémon is "Super Shiny,"
    /// and false signifies it is not.
    /// </value>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess", BlueprintAccessors = true)]
    public bool IsSuperShiny
    {
        get => IsSuperShiny_BackingField;
        set
        {
            IsSuperShiny_BackingField = value;
            if (value)
                IsShiny = true;
        }
    }

    /// <summary>
    /// Initializes the identity component of a Pokémon using the provided initialization parameters.
    /// </summary>
    /// <param name="initParams">The initialization parameters containing species and other relevant data.</param>
    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        Species = initParams.Species;
        Nickname = Species.Entry.Name;
        var personalityValue =
            (uint)Random.Shared.Next(ushort.MaxValue + 1) | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
        PersonalityValue = personalityValue;

        var species = Species.Entry;
        Gender = species.GenderRatio.Entry.Ratio.Match(
            femaleChance => (personalityValue & 0xFF) < femaleChance ? EPokemonGender.Female : EPokemonGender.Male,
            gender => gender
        );

        var personality = PersonalityValue;
        var ownerId = OwningEntity.Outer is UTrainer trainer ? trainer.ID : Random.Shared.Next();

        var shininessValue = GetShininessValue(personality, ownerId);
        IsShiny = shininessValue < PokemonStatics.ShininessChance;
        IsSuperShiny = shininessValue == 0;
    }

    /// <summary>
    /// Calculates the shininess value for a Pokémon based on its personality and the owner's unique identifier.
    /// </summary>
    /// <param name="personality">The personality value of the Pokémon.</param>
    /// <param name="ownerId">The unique identifier of the Pokémon's owner.</param>
    /// <returns>Returns the computed shininess value.</returns>
    protected virtual long GetShininessValue(uint personality, int ownerId)
    {
        var a = personality ^ ownerId;
        var b = a & 0xFFFF;
        var c = (a >> 16) & 0xFFFF;
        return b ^ c;
    }
}
