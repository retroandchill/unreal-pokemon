using JetBrains.Annotations;
using Pokemon.Core.Entities;
using Pokemon.Data;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;
using ZLinq;

namespace Pokemon.Core.Components.Pokemon;

/// <summary>
/// Represents a pairing of an ability and its associated index.
/// </summary>
/// <remarks>
/// This structure is used to associate an ability handle with a specific index, which can represent
/// the position of the ability in a list or its classification (e.g., regular or hidden ability).
/// </remarks>
[UStruct]
public readonly partial record struct FAbilityPair(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FAbilityHandle Ability,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Index
);

/// <summary>
/// Represents the ability management component for a Pokémon entity.
/// </summary>
/// <remarks>
/// This component facilitates the handling of abilities for a Pokémon, allowing for the retrieval,
/// modification, and initialization of abilities, including distinguishing between regular and hidden abilities.
/// It supports both explicitly set abilities and abilities determined based on the Pokémon's identity and properties.
/// </remarks>
[UClass]
[UMetaData("HideCategories", "Abilities")]
[UsedImplicitly]
public partial class UAbilityComponent : URPGComponent
{
    /// <summary>
    /// Represents the maximum number of regular abilities a Pokémon can have.
    /// </summary>
    /// <remarks>
    /// This constant defines the upper limit of abilities categorized as "regular abilities" for a Pokémon species.
    /// It is used in distinguishing regular abilities from hidden abilities, as well as for determining valid ability assignments.
    /// </remarks>
    public const int MaxRegularAbility = 2;

    [UProperty(PropertyFlags.Transient)]
    private UIdentityComponent IdentityComponent { get; set; }

    /// <summary>
    /// Represents the index of the current active ability for a Pokémon.
    /// </summary>
    /// <remarks>
    /// The ability index determines which specific ability, either regular or hidden, is currently selected or being used.
    /// This property interacts with the Pokémon's species and ability configuration to identify and assign the proper ability.
    /// It supports differentiation between regular abilities (indexed from 0) and hidden abilities (indexed starting from the maximum regular ability limit).
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Abilities")]
    public int AbilityIndex { get; set; }

    /// <summary>
    /// Represents an explicitly assigned ability for a Pokémon, overriding default or random abilities.
    /// </summary>
    /// <remarks>
    /// This property allows for explicitly setting an ability for a Pokémon, bypassing the standard mechanism
    /// by which abilities are assigned. When an explicit ability is set, it takes precedence over other
    /// abilities, such as those determined by the Pokémon species or randomly selected abilities.
    /// Used in scenarios where direct control over a Pokémon's ability is required.
    /// </remarks>
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Abilities")]
    public TOptional<FAbilityHandle> ExplicitAbility { get; set; }

    /// <summary>
    /// Represents the ability currently assigned to the Pokémon.
    /// </summary>
    /// <remarks>
    /// Determines the active ability of a Pokémon based on either an explicitly set ability or the species' default abilities.
    /// If an explicit ability is set, it takes precedence; otherwise, it calculates the ability from the species' regular or hidden abilities.
    /// </remarks>
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
                            ? species.Abilities[Math.Min(AbilityIndex, species.Abilities.Length - 1)]
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

    /// <summary>
    /// Determines whether the Pokémon currently has an assigned valid ability.
    /// </summary>
    /// <remarks>
    /// This property checks if the Pokémon's ability is set and valid. A valid ability is one
    /// that has been initialized and is considered functional within the game's ability system.
    /// It helps in verifying whether the Pokémon is effectively utilizing an ability.
    /// </remarks>
    public bool HasAbility
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Has Ability", Category = "Abilities")]
        get => Ability.IsValid;
    }

    /// <summary>
    /// Indicates whether the Pokémon possesses a hidden ability.
    /// </summary>
    /// <remarks>
    /// This property determines if the Pokémon has a hidden ability by evaluating whether the current ability index
    /// exceeds the maximum number of regular abilities. Hidden abilities are categorized separately from regular abilities
    /// and are often considered unique or rare attributes of a Pokémon.
    /// </remarks>
    public bool HasHiddenAbility
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Has Hidden Ability", Category = "Abilities")]
        get => AbilityIndex >= MaxRegularAbility;
    }

    /// <summary>
    /// Provides a read-only collection of all abilities a Pokémon can potentially possess,
    /// including both regular and hidden abilities.
    /// </summary>
    /// <remarks>
    /// This property aggregates the Pokémon's species-defined abilities and hidden abilities
    /// into a unified list of ability-index pairs. Each pair contains the ability along with its
    /// index, where regular abilities use indices starting from 0 and hidden abilities have indices
    /// offset by the maximum number of regular abilities.
    /// </remarks>
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

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        IdentityComponent = GetRequiredSiblingComponent<UIdentityComponent>();
    }

    /// <summary>
    /// Initializes the ability component using the provided Pokémon initialization parameters.
    /// </summary>
    /// <param name="initParams">The initialization parameters for Pokémon, including species and level.</param>
    [UFunction]
    [ExcludeFromExtensions]
    public void Initialize(FPokemonInitParams initParams)
    {
        AbilityIndex = (int)IdentityComponent.PersonalityValue % MaxRegularAbility;
    }
}
