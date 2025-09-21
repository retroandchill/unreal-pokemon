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

    public IEnumerable<UPokemon> AbleParty => Party.Where(p => p.IsAbleToBattle);

    public int PartyCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party.Count;
    }
    
    public int AblePokemonCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.Count();
    }

    public bool IsPartyFull
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Party Full", Category = "Party")]
        get => Party.Count >= PokemonStatics.MaxPartySize;
    }

    public bool IsPartyEmpty
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Party Empty", Category = "Party")]
        get => Party.Count == 0;
    }

    public bool AllPokemonUnableToBattle
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AblePokemonCount == 0;
    }

    public UPokemon FirstPartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party[0];
    }

    public UPokemon? FirstAblePartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.FirstOrDefault();
    }
    
    public UPokemon LastPartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party[^1];
    }
    
    public UPokemon? LastAblePartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.LastOrDefault();
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

    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public UPokemon? GetPokemon(int index)
    {
        return index >= 0 && index < Party.Count ? Party[index] : null;
    }

    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public void AddPokemon(UPokemon pokemon)
    {
        if (IsPartyFull)
        {
            throw new InvalidOperationException("Party is full");
        }
        
        Party.Add(pokemon);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Party")]
    public bool RemovePokemonAtIndex(int index)
    {
        ArgumentOutOfRangeException.ThrowIfLessThan(index, 0);
        ArgumentOutOfRangeException.ThrowIfGreaterThan(index, Party.Count - 1);

        if (!HasOtherAblePokemon(index))
        {
            return false;
        }
        
        Party.RemoveAt(index);
        return true;
    }

    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public bool HasOtherAblePokemon(int index)
    {
        return Party.Index().Any(x => x.Index != index && x.Item.IsAbleToBattle);
    }
    
    [UFunction]
    public void SwapPositionsInParty(int index1, int index2)
    {
        Party.Swap(index1, index2);
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Party")]
    public void HealParty()
    {
        foreach (var pokemon in Party)
        {
            pokemon.Heal();
        }
    }

    protected override void PreInitialize()
    {
        InfoComponent = GetRequiredSiblingComponent<UTrainerInfoComponent>();
    }
}
