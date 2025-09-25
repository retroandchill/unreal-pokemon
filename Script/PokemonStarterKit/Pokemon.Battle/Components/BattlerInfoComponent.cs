using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Battle.Components;

[UClass]
public class UBattlerInfoComponent : URPGComponent
{
    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public UPokemon Pokemon { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public FText Name { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public FSpeciesHandle Species { get; set; }

    [UProperty(PropertyFlags.BlueprintReadWrite, Category = "Battler|Info")]
    public TArray<FTypeHandle> Types { get; set; }
}
