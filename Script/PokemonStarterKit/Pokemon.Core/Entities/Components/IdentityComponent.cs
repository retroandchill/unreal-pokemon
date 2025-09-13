using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities.Components;

[UClass]
[UMetaData("HideCategories", "Identity")]
public class UIdentityComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public FSpeciesHandle Species { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public FText Nickname { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public uint PersonalityValue { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public int ID { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Identity")]
    public int SecretID { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Name", Category = "Identity")]
    public FText OTName { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, DisplayName = "OT Gender", Category = "Identity")]
    public ETrainerGender OTGender { get; set; }

    [UFunction]
    [UMetaData("RPGInitFunction")]
    private void Initialize(FPokemonInitParams initParams)
    {
        Species = initParams.Species;
        PersonalityValue =
            (uint)Random.Shared.Next(ushort.MaxValue + 1)
            | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
    }
}
