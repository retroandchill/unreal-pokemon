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

[UEnum]
public enum EObtainMethod : byte
{
    Default,
    Egg,
    Trade,
    FatefulEncounter,
}

[UStruct]
public readonly partial record struct FHatchInformation(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FDateTime TimeHatched,
    FText HatchText
);

[UClass]
public class UOwnershipComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public UTrainer Owner { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public int ID { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public int SecretID { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Name", Category = "Identity")]
    public FText OTName { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Gender", Category = "Identity")]
    public ETrainerGender OTGender { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public EObtainMethod ObtainMethod { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public int LevelMet { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public FDateTime TimeReceived { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public FText ObtainText { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Ownership")]
    public Option<FHatchInformation> HatchInformation { get; set; }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        var trainer = (UTrainer)SystemLibrary.GetOuterObject(OwningEntity);
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
