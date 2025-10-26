using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Trainer;

/// <summary>
/// Represents the core component that encapsulates trainer information in the system.
/// This class is used to store and manage data related to a trainer,
/// including type, name, ID, secret ID, and gender. It provides utility functions
/// to initialize the trainer's information and calculate derived properties.
/// Inherits from URPGComponent.
/// </summary>
[UClass]
[UMetaData("HideCategories", "Trainer")]
public partial class UTrainerInfoComponent : URPGComponent
{
    /// <summary>
    /// Represents a handle to the trainer type associated with the trainer.
    /// This property serves as a reference to the trainer type in the game's data model,
    /// and it provides access to properties and metadata specific to the trainer type.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Trainer")]
    public partial FTrainerTypeHandle TrainerType { get; set; }

    /// <summary>
    /// Represents the name of the trainer in the system.
    /// This property holds the textual representation of the trainer's name,
    /// used in conjunction with other trainer attributes such as type and ID.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Trainer")]
    public partial FText Name { get; set; }

    /// <summary>
    /// Gets the full name of the trainer, consisting of the trainer type and the trainer's name.
    /// This property combines the specific trainer type designation with the user-defined name
    /// to provide a complete representation of the trainer's identity.
    /// </summary>
    public FText FullName
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get
        {
            ReadOnlySpan<char> trainerTypeName = TrainerType.Entry.Name;
            ReadOnlySpan<char> trainerName = Name;
            return new FText($"{trainerTypeName} {trainerName}");
        }
    }

    /// <summary>
    /// Represents the gender associated with the trainer.
    /// This property indicates the trainer's gender as defined in the data model
    /// and is used to differentiate trainers based on their assigned or configured gender.
    /// Possible values include Male, Female, Unknown, and Mixed.
    /// </summary>
    public ETrainerGender Gender
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => TrainerType.Entry.Gender;
    }

    [UProperty]
    public partial uint FullID { get; set; }

    private const uint TrainerIdMask = 1000000;

    /// <summary>
    /// Represents the unique identifier for a trainer.
    /// This property extracts the trainer's ID portion from the full identifier,
    /// which combines both the ID and secret ID.
    /// </summary>
    public int ID
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => (int)(FullID % TrainerIdMask);
    }

    /// <summary>
    /// Represents the secret ID of the trainer as part of their unique identity in the system.
    /// This numerical value is derived from the full trainer ID and is used
    /// for various operations where differentiation between trainers is necessary.
    /// </summary>
    public int SecretID
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => (int)(FullID / TrainerIdMask);
    }

    /// <summary>
    /// Initializes the trainer information using the provided parameters.
    /// </summary>
    /// <param name="parameters">The parameters used to initialize the trainer, including trainer type and name.</param>
    [UFunction]
    public void Initialize(FTrainerInitializer parameters)
    {
        TrainerType = parameters.TrainerType;
        Name = parameters.Name;
        FullID = (uint)Random.Shared.Next(ushort.MaxValue + 1) | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
    }
}
