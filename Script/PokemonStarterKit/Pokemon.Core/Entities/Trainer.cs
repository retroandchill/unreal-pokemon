using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Components.Trainer;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

[UStruct]
public readonly partial record struct FTrainerInitializer(
    FTrainerTypeHandle TrainerType,
    FText Name
);

[UClass(ClassFlags.Abstract)]
public partial class UTrainer : URPGEntity
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UTrainerInfoComponent.Initialize))]
    public UTrainerInfoComponent InfoComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UPartyComponent PartyComponent { get; private set; }

    public static UTrainer Create(UObject outer, FTrainerTypeHandle trainerType, FText name)
    {
        return Create(outer, new FTrainerInitializer(trainerType, name));
    }

    [Factory(SubclassSource = nameof(TrainerClass))]
    public static partial UTrainer Create(UObject outer, FTrainerInitializer parameters);

    private static TSubclassOf<UTrainer> TrainerClass =>
        GetDefault<UPokemonCoreSettings>().TrainerClass.LoadSynchronous();
}
