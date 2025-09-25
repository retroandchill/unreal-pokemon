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

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Gender")]
    public EPokemonGender Gender
    {
        get;
        set
        {
            if (IsSingleGendered)
                return;

            field = value;
        }
    }

    public bool IsSingleGendered
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Species.Entry.GenderRatio.Entry.Ratio.IsSingleGender;
    }

    public bool IsMale
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Male;
    }

    public bool IsFemale
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Female;
    }

    public bool IsGenderless
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Gender")]
        get => Gender == EPokemonGender.Genderless;
    }

    public ReadOnlySpan<FTypeHandle> Types => Species.Entry.Types;

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess")]
    public bool IsShiny { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Shininess")]
    public bool IsSuperShiny
    {
        get;
        set
        {
            field = value;
            if (value)
                IsShiny = true;
        }
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        Species = initParams.Species;
        Nickname = Species.Entry.Name;
        var personalityValue =
            (uint)Random.Shared.Next(ushort.MaxValue + 1)
            | (uint)(Random.Shared.Next(ushort.MaxValue + 1) << 16);
        PersonalityValue = personalityValue;

        var species = Species.Entry;
        Gender = species.GenderRatio.Entry.Ratio.Match(
            femaleChance =>
                (personalityValue & 0xFF) < femaleChance
                    ? EPokemonGender.Female
                    : EPokemonGender.Male,
            gender => gender
        );

        var personality = PersonalityValue;
        var ownerId = OwningEntity.Outer is UTrainer trainer ? trainer.ID : Random.Shared.Next();

        var shininessValue = GetShininessValue(personality, ownerId);
        IsShiny = shininessValue < PokemonStatics.ShininessChance;
        IsSuperShiny = shininessValue == 0;
    }

    protected virtual long GetShininessValue(uint personality, int ownerId)
    {
        var a = personality ^ ownerId;
        var b = a & 0xFFFF;
        var c = (a >> 16) & 0xFFFF;
        return b ^ c;
    }
}
