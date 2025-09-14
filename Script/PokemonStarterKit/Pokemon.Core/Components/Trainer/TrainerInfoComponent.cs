using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Trainer;

[UClass]
[UMetaData("HideCategories", "Trainer")]
public class UTrainerInfoComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Trainer")]
    public FTrainerTypeHandle TrainerType { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Trainer")]
    public FText Name { get; set; }

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

    public ETrainerGender Gender
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => TrainerType.Entry.Gender;
    }

    [UProperty]
    private uint FullID { get; set; }

    private const uint TrainerIdMask = 1000000;

    public int ID
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => (int)(FullID % TrainerIdMask);
    }

    public int SecretID
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get => (int)(FullID / TrainerIdMask);
    }

    [UFunction]
    public void Initialize(FTrainerInitializer parameters)
    {
        TrainerType = parameters.TrainerType;
        Name = parameters.Name;
        FullID =
            (uint)Random.Shared.Next(ushort.MaxValue + 1)
            | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
    }
}
