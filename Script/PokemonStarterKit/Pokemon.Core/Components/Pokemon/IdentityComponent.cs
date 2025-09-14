using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

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

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        Species = initParams.Species;
        PersonalityValue =
            (uint)Random.Shared.Next(ushort.MaxValue + 1)
            | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
    }
}
