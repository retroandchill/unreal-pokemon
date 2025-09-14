using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.HardCoded;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Pokemon;

[UClass]
[UMetaData("HideCategories", "Gender")]
[UsedImplicitly]
public class UGenderComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

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
        get => IdentityComponent.Species.Entry.GenderRatio.Entry.Ratio.IsSingleGender;
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

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        var species = initParams.Species.Entry;
        Gender = species.GenderRatio.Entry.Ratio.Match(
            femaleChance =>
                (IdentityComponent.PersonalityValue & 0xFF) < femaleChance
                    ? EPokemonGender.Female
                    : EPokemonGender.Male,
            gender => gender
        );
    }
}
