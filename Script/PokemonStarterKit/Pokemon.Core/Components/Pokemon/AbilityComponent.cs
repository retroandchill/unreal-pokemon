using JetBrains.Annotations;
using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Data;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using ZLinq;

namespace Pokemon.Core.Components.Pokemon;

[UStruct]
public readonly partial record struct FAbilityPair(
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] FAbilityHandle Ability,
    [field: UProperty(PropertyFlags.BlueprintReadOnly)] int Index
);

[UClass]
[UMetaData("HideCategories", "Abilities")]
[UsedImplicitly]
public class UAbilityComponent : URPGComponent
{
    public const int MaxRegularAbility = 2;

    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Abilities")]
    public int AbilityIndex { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Abilities")]
    public Option<FAbilityHandle> ExplicitAbility { get; set; }

    public FAbilityHandle Ability
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Abilities")]
        get
        {
            return ExplicitAbility.Match(
                x => x,
                () =>
                {
                    var species = IdentityComponent.Species.Entry;
                    if (AbilityIndex < MaxRegularAbility)
                        return species.Abilities.Length != 0
                            ? species.Abilities[
                                Math.Min(AbilityIndex, species.Abilities.Length - 1)
                            ]
                            : default;

                    var hiddenAbilityIndex = AbilityIndex - MaxRegularAbility;
                    return species.HiddenAbilities.Length < hiddenAbilityIndex
                        ? species.HiddenAbilities[hiddenAbilityIndex]
                        : default;
                }
            );
        }
        [UFunction(FunctionFlags.BlueprintCallable, Category = "Abilities")]
        set
        {
            if (!value.IsValid)
                return;

            ExplicitAbility = value;
        }
    }

    public bool HasAbility
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Has Ability",
            Category = "Abilities"
        )]
        get => Ability.IsValid;
    }

    public bool HasHiddenAbility
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Has Hidden Ability",
            Category = "Abilities"
        )]
        get => AbilityIndex >= MaxRegularAbility;
    }

    public IReadOnlyList<FAbilityPair> PossibleAbilities
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Abilities")]
        get
        {
            var species = IdentityComponent.Species.Entry;
            return species
                .Abilities.AsValueEnumerable()
                .Select((x, i) => new FAbilityPair(x, i))
                .Concat(
                    species
                        .HiddenAbilities.AsValueEnumerable()
                        .Select((x, i) => new FAbilityPair(x, i + MaxRegularAbility))
                )
                .ToArray();
        }
    }

    protected override void PreInitialize()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        AbilityIndex = (int)IdentityComponent.PersonalityValue % MaxRegularAbility;
    }
}
