using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUtilities;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Represents the method by which an in-game entity, such as a Pokémon, was obtained.
/// </summary>
[UEnum]
public enum EObtainMethod : byte
{
    /// <summary>
    /// The default method for obtaining an entity.
    /// </summary>
    Default,

    /// <summary>
    /// The Pokémon was hatched from an egg.
    /// </summary>
    Egg,

    /// <summary>
    /// The Pokémon was obtained through a trade.
    /// </summary>
    Trade,

    /// <summary>
    /// The encounter was a fateful encounter, meaning it was through an event or a special encounter.
    /// </summary>
    FatefulEncounter,
}

/// <summary>
/// Represents hatch-related information associated with a Pokémon or similar in-game entity.
/// </summary>
/// <param name="TimeHatched">The time the egg hatched</param>
/// <param name="HatchText">The text describing the hatch event</param>
[UStruct]
public readonly partial record struct FHatchInformation(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FDateTime TimeHatched,
    FText HatchText
);

/// <summary>
/// Represents a component responsible for managing ownership-related information of a Pokémon entity.
/// </summary>
[UClass]
[UMetaData("HideCategories", "Ownership,OriginalTrainer,Obtain")]
public class UOwnershipComponent : URPGComponent
{
    /// <summary>
    /// Gets or sets the trainer that owns the Pokémon.
    /// This property links the Pokémon to its respective owner or trainer entity.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public UTrainer Owner { get; set; }

    /// <summary>
    /// Gets or sets the unique identifier for the Pokémon.
    /// This property represents the numerical ID used to uniquely identify the Pokémon
    /// within the context of the ownership or trainer system.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "OriginalTrainer")]
    public int ID { get; set; }

    /// <summary>
    /// Gets or sets the secret identification number associated with the trainer.
    /// This property uniquely identifies the trainer in conjunction with their ID
    /// and is commonly used for Pokémon ownership verification purposes.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "OriginalTrainer")]
    public int SecretID { get; set; }

    /// <summary>
    /// Gets or sets the original trainer's name associated with the Pokémon.
    /// This property identifies the name of the trainer who originally owned the Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Name", Category = "OriginalTrainer")]
    public FText OTName { get; set; }

    /// <summary>
    /// Gets or sets the gender of the original trainer who owned the Pokémon.
    /// This property specifies the trainer's gender using defined enumeration values.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Gender", Category = "OriginalTrainer")]
    public ETrainerGender OTGender { get; set; }

    /// <summary>
    /// Gets or sets the method by which the Pokémon was obtained.
    /// This property indicates whether the Pokémon was acquired through default means,
    /// hatching from an egg, trading with another trainer, or as a special fateful encounter.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Obtain")]
    public EObtainMethod ObtainMethod { get; set; }

    /// <summary>
    /// Gets or sets the level at which this Pokémon was encountered or obtained.
    /// This property represents the level information relevant to the Pokémon's origin.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Obtain")]
    public int LevelMet { get; set; }

    /// <summary>
    /// Gets or sets the date and time when the Pokémon was received.
    /// This property records the moment the Pokémon was obtained by its current owner.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Obtain")]
    public FDateTime TimeReceived { get; set; }

    /// <summary>
    /// Gets or sets the text description of how the Pokémon was obtained.
    /// This property provides additional contextual information about the method or location associated with acquiring the Pokémon.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Obtain")]
    public FText ObtainText { get; set; }

    /// <summary>
    /// Gets or sets information about the hatching of the Pokémon.
    /// This property captures details such as the time the Pokémon was hatched
    /// and any associated text describing the hatching process or circumstances.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Obtain")]
    public Option<FHatchInformation> HatchInformation { get; set; }

    /// <summary>
    /// Initializes the ownership component of a Pokémon with the specified parameters.
    /// </summary>
    /// <param name="initParams">
    /// The initialization parameters for the Pokémon, containing species data and its starting level.
    /// </param>
    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        var trainer = (UTrainer)OwningEntity.Outer;
        Owner = trainer;
        ID = trainer.ID;
        SecretID = trainer.SecretID;
        OTName = trainer.Name;
        OTGender = trainer.Gender;

        ObtainMethod = EObtainMethod.Default;
        LevelMet = initParams.Level;
        TimeReceived = FDateTime.Now;

        var subsystem = GetGameInstanceSubsystem<UPokemonSubsystem>();
        ObtainText = subsystem.CurrentLocation;
    }
}
