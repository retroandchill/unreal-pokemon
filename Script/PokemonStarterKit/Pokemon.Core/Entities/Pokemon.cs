using GameDataAccessTools.Core.Utilities;
using Pokemon.Core.Entities.Components;
using Pokemon.Data.Model.PBS;
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
public class UPokemon : URPGEntity
{
    protected override UScriptStruct ManagedGetEntityStruct() => FPokemonInitParams.StaticStruct;

    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public UIdentityComponent IdentityComponent { get; }

    public static UPokemon Create(UObject outer, FSpeciesHandle species, int level)
    {
        return Create(outer, new FPokemonInitParams(species, level));
    }

    public static UPokemon Create(UObject outer, FPokemonInitParams @params)
    {
        var newPokemon = NewObject<UPokemon>(outer);
        newPokemon.InitializeComponents(@params);
        return newPokemon;
    }
}
