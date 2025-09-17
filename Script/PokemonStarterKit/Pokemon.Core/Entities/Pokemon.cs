using JetBrains.Annotations;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Engine;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

[UStruct]
public readonly partial record struct FPokemonInitParams(
    [field: UProperty] FSpeciesHandle Species,
    [field: UProperty] int Level
);

[UClass(ClassFlags.Abstract)]
[UsedImplicitly]
public partial class UPokemon : URPGEntity
{
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UIdentityComponent.Initialize))]
    public UIdentityComponent IdentityComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UOwnershipComponent.Initialize))]
    public UOwnershipComponent OwnershipComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UGenderComponent.Initialize))]
    public UGenderComponent GenderComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UShininessComponent.Initialize))]
    public UShininessComponent ShininessComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UStatComponent.Initialize))]
    public UStatComponent StatComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UAbilityComponent.Initialize))]
    public UAbilityComponent AbilityComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UMoveComponent.Initialize))]
    public UMoveComponent MoveComponent { get; private set; }

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UHoldItemComponent HoldItemComponent { get; private set; }
    
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UStatusEffectComponent StatusEffectComponent { get; private set; }

    [PublicAPI]
    public static UPokemon Create(UTrainer outer, FSpeciesHandle species, int level)
    {
        return Create(outer, new FPokemonInitParams(species, level));
    }

    [Factory]
    [PublicAPI]
    public static partial UPokemon Create(UTrainer outer, FPokemonInitParams parameters);

    private static TSubclassOf<UPokemon> PokemonClass =>
        GetDefault<UPokemonCoreSettings>().PokemonClass.LoadSynchronous();
}
