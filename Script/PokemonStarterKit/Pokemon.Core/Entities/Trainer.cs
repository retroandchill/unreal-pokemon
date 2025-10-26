using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Components.Trainer;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

/// <summary>
/// Represents the initializer for a Pokémon trainer, containing the trainer's type and name.
/// </summary>
[UStruct]
public readonly partial record struct FTrainerInitializer(FTrainerTypeHandle TrainerType, FText Name);

/// <summary>
/// Represents a base class for a Pokémon trainer entity, encapsulating trainer-related components and behavior.
/// </summary>
[UClass(ClassFlags.Abstract)]
public partial class UTrainer : URPGEntity
{
    /// <summary>
    /// Gets the trainer's information component, which encapsulates the trainer's characteristics,
    /// including their details such as type, name, and unique identifier. This property is initialized
    /// using the <see cref="UTrainerInfoComponent.Initialize"/> method.
    /// </summary>
    /// <value>
    /// The <see cref="UTrainerInfoComponent"/> instance associated with the trainer.
    /// </value>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UTrainerInfoComponent.Initialize))]
    public partial UTrainerInfoComponent InfoComponent { get; private set; }

    /// <summary>
    /// Gets the party component of the trainer, which manages the trainer's Pokémon party,
    /// including details like party members, their ability to battle, and overall party status.
    /// </summary>
    /// <value>
    /// The <see cref="UPartyComponent"/> instance associated with the trainer's party management.
    /// </value>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UPartyComponent PartyComponent { get; private set; }

    /// <summary>
    /// Creates a new instance of a UTrainer object using the specified outer object, trainer type, and name.
    /// </summary>
    /// <param name="outer">The UObject that serves as the outer context for the new trainer instance.</param>
    /// <param name="trainerType">The handle representing the type of trainer to be created.</param>
    /// <param name="name">The name of the trainer to be created.</param>
    /// <returns>A new instance of UTrainer initialized with the specified parameters.</returns>
    public static UTrainer Create(UObject outer, FTrainerTypeHandle trainerType, FText name)
    {
        return Create(outer, new FTrainerInitializer(trainerType, name));
    }

    /// <summary>
    /// Creates a new instance of a UTrainer object using the specified outer object, trainer type, and name.
    /// </summary>
    /// <param name="outer">The UObject that acts as the outer context for the new trainer instance.</param>
    /// <param name="parameters">The parameters used for initialization</param>
    /// <returns>A new instance of UTrainer initialized with the specified parameters.</returns>
    [Factory(SubclassSource = nameof(TrainerClass))]
    public static partial UTrainer Create(UObject outer, FTrainerInitializer parameters);

    private static TSubclassOf<UTrainer> TrainerClass =>
        GetDefault<UPokemonCoreSettings>().TrainerClass.LoadSynchronous();
}
