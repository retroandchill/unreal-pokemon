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
public partial class UPokemon : URPGEntity
{
    public UTrainer Trainer
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Pokémon")]
        get => (UTrainer)SystemLibrary.GetOuterObject(this);
    }

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

    public static UPokemon Create(UTrainer outer, FSpeciesHandle species, int level)
    {
        return Create(outer, new FPokemonInitParams(species, level));
    }

    [Factory]
    public static partial UPokemon Create(UTrainer outer, FPokemonInitParams parameters);

    private static TSubclassOf<UPokemon> PokemonClass =>
        GetDefault<UPokemonCoreSettings>().PokemonClass.LoadSynchronous();
}
