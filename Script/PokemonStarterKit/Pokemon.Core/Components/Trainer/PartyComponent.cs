using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Trainer;

[UClass]
[UMetaData("HideCategories", "Trainer,Party")]
public class UPartyComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private UTrainerInfoComponent InfoComponent { get; set; }

    [UProperty]
    private TArray<UPokemon> Party { get; }

    public IReadOnlyList<UPokemon> PartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party;
    }

    public int Payout
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get
        {
            ArgumentOutOfRangeException.ThrowIfZero(Party.Count);
            return InfoComponent.TrainerType.Entry.BaseMoney * Party[^1].Level;
        }
    }

    public bool IsPartyFull
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party.Count >= PokemonStatics.MaxPartySize;
    }

    [UFunction]
    public void SwapPositionsInParty(int index1, int index2)
    {
        Party.Swap(index1, index2);
    }

    protected override void PreInitialize()
    {
        InfoComponent = GetRequiredSiblingComponent<UTrainerInfoComponent>();
    }
}
