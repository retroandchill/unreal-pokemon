using Pokemon.Core.Components.Pokemon;
using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Components.Trainer;

/// <summary>
/// Represents a trainer's party component, which manages the Pokémon in the party
/// and provides various utility methods for accessing and managing them.
/// </summary>
[UClass]
[UMetaData("HideCategories", "Trainer,Party")]
public partial class UPartyComponent : URPGComponent
{
    [UProperty(PropertyFlags.Transient)]
    private partial UTrainerInfoComponent InfoComponent { get; set; }

    [UProperty]
    private partial TArray<UPokemon> Party { get; }

    /// <summary>
    /// Provides a read-only list of Pokémon in the trainer's active party.
    /// Allows external access to the collection without permitting modification.
    /// </summary>
    public IReadOnlyList<UPokemon> PartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party;
    }

    /// <summary>
    /// Provides a filtered collection of Pokémon within the trainer's party that are currently able to participate in battle.
    /// Only includes Pokémon with the capability to engage in battle.
    /// </summary>
    public IEnumerable<UPokemon> AbleParty => Party.Where(p => p.IsAbleToBattle);

    /// <summary>
    /// Retrieves the total number of Pokémon currently present in the trainer's party.
    /// Useful to determine the party's size for various gameplay or UI-related purposes.
    /// </summary>
    public int PartyCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party.Count;
    }

    /// <summary>
    /// Gets the number of Pokémon in the trainer's party that are able to battle.
    /// This count excludes fainted or otherwise incapacitated Pokémon.
    /// </summary>
    public int AblePokemonCount
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.Count();
    }

    /// <summary>
    /// Determines whether the trainer's party has reached the maximum allowed size.
    /// Returns true if the party contains the maximum number of Pokémon, as defined by the game settings.
    /// </summary>
    public bool IsPartyFull
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Party Full", Category = "Party")]
        get => Party.Count >= PokemonStatics.MaxPartySize;
    }

    /// <summary>
    /// Indicates whether the trainer's party is empty, returning true if there
    /// are no Pokémon present in the party.
    /// </summary>
    public bool IsPartyEmpty
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Party Empty", Category = "Party")]
        get => Party.Count == 0;
    }

    /// <summary>
    /// Determines if all Pokémon in the trainer's party are unable to battle.
    /// Provides a boolean value indicating whether there are no Pokémon in the party
    /// that can currently participate in battle.
    /// </summary>
    public bool AllPokemonUnableToBattle
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AblePokemonCount == 0;
    }

    /// <summary>
    /// Retrieves the first Pokémon in the trainer's party.
    /// This property provides direct access to the Pokémon at the first position in the party list.
    /// </summary>
    public UPokemon FirstPartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party[0];
    }

    /// <summary>
    /// Retrieves the first Pokémon in the trainer's party that is able to battle.
    /// If no such Pokémon exists, returns null.
    /// </summary>
    public UPokemon? FirstAblePartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.FirstOrDefault();
    }

    /// <summary>
    /// Retrieves the last Pokémon in the trainer's active party.
    /// This property provides direct access to the Pokémon at the last position in the party list.
    /// </summary>
    public UPokemon LastPartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => Party[^1];
    }

    /// <summary>
    /// Retrieves the last Pokémon in the trainer's party that is able to participate in battles.
    /// Returns null if no Pokémon in the party are capable of battling.
    /// </summary>
    public UPokemon? LastAblePartyPokemon
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
        get => AbleParty.LastOrDefault();
    }

    /// <summary>
    /// Retrieves the payout value for a trainer, calculated based on the trainer's type
    /// and the level of the last Pokémon in the party.
    /// Throws an exception if the party is empty.
    /// </summary>
    public int Payout
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Trainer")]
        get
        {
            ArgumentOutOfRangeException.ThrowIfZero(Party.Count);
            return InfoComponent.TrainerType.Entry.BaseMoney * Party[^1].Level;
        }
    }

    /// <summary>
    /// Retrieves a Pokémon from the party at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index of the Pokémon to retrieve in the party.</param>
    /// <returns>
    /// The Pokémon located at the specified index in the party,
    /// or <c>null</c> if the index is out of range.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public UPokemon? GetPokemon(int index)
    {
        return index >= 0 && index < Party.Count ? Party[index] : null;
    }

    /// <summary>
    /// Adds a Pokémon to the party.
    /// </summary>
    /// <param name="pokemon">The Pokémon to be added to the party.</param>
    /// <exception cref="InvalidOperationException">
    /// Thrown if the party is full and cannot accommodate additional Pokémon.
    /// </exception>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public void AddPokemon(UPokemon pokemon)
    {
        if (IsPartyFull)
        {
            throw new InvalidOperationException("Party is full");
        }

        Party.Add(pokemon);
    }

    /// <summary>
    /// Removes the Pokémon at the specified index in the party if other Pokémon are still able to battle.
    /// </summary>
    /// <param name="index">The zero-based index of the Pokémon to remove from the party.</param>
    /// <returns>
    /// <c>true</c> if the Pokémon was successfully removed;
    /// otherwise, <c>false</c> if no removal occurred because no other Pokémon in the party can battle.
    /// </returns>
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

    /// <summary>
    /// Determines whether there are other Pokémon in the party able to battle, excluding the Pokémon at the specified index.
    /// </summary>
    /// <param name="index">The zero-based index of the Pokémon to exclude from the check.</param>
    /// <returns>
    /// <c>true</c> if there is at least one other Pokémon in the party able to battle;
    /// otherwise, <c>false</c>.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintPure, Category = "Party")]
    public bool HasOtherAblePokemon(int index)
    {
        return Party.Index().Any(x => x.Index != index && x.Item.IsAbleToBattle);
    }

    /// <summary>
    /// Swaps the positions of two Pokémon in the party at the specified indices.
    /// </summary>
    /// <param name="index1">The zero-based index of the first Pokémon in the party.</param>
    /// <param name="index2">The zero-based index of the second Pokémon in the party.</param>
    [UFunction]
    public void SwapPositionsInParty(int index1, int index2)
    {
        Party.Swap(index1, index2);
    }

    /// <summary>
    /// Heals all Pokémon in the party, restoring their health and status conditions.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Party")]
    public void HealParty()
    {
        foreach (var pokemon in Party)
        {
            pokemon.Heal();
        }
    }

    /// <inheritdoc />
    protected override void PreInitialize_Implementation()
    {
        InfoComponent = GetRequiredSiblingComponent<UTrainerInfoComponent>();
    }
}
