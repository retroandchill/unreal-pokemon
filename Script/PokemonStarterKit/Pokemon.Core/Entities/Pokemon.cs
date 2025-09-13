using GameDataAccessTools.Core.Utilities;
using Pokemon.Core.Entities.Components;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
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
    protected override UScriptStruct ManagedGetEntityStruct() => FPokemonInitParams.StaticStruct;

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

    public static UPokemon Create(UObject outer, FSpeciesHandle species, int level)
    {
        return Create(outer, new FPokemonInitParams(species, level));
    }

    [Factory]
    public static partial UPokemon Create(UObject outer, FPokemonInitParams parameters);

    private static TSubclassOf<UPokemon> PokemonClass =>
        GetDefault<UPokemonCoreSettings>().PokemonClass;
}
