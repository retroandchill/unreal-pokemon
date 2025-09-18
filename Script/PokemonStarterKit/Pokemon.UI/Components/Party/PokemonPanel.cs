using LanguageExt;
using Pokemon.Core.Entities;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CommonUI;

namespace Pokemon.UI.Components.Party;

public interface IPokemonPanelOwner
{
    int? SwitchingIndex { get; }

    bool IsSwitching => SwitchingIndex.HasValue;
}

[UClass(ClassFlags.Abstract)]
public class UPokemonPanel : UCommonButtonBase
{
    public IPokemonPanelOwner? Owner { get; set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Content")]
    public UPokemon? Pokemon { get; private set; }

    private int _pokemonIndex;

    public bool IsActivate
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Active", Category = "Display")]
        get => _pokemonIndex == 0;
    }

    public bool IsPokemonFainted
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Pokémon Fainted",
            Category = "Display"
        )]
        get => Pokemon is not null && Pokemon.IsFainted;
    }

    public bool IsPanelSelected
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Panel Selected",
            Category = "Display"
        )]
        get => IsHovered();
    }

    public bool IsSwapping
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Swapping", Category = "Display")]
        get => Owner?.IsSwitching ?? false;
    }

    public bool IsPreselected
    {
        [UFunction(
            FunctionFlags.BlueprintPure,
            DisplayName = "Is Preselected",
            Category = "Display"
        )]
        get => Owner?.SwitchingIndex == _pokemonIndex;
    }

    public void SwapPokemon(UPokemonPanel other)
    {
        (Pokemon, other.Pokemon) = (other.Pokemon, Pokemon);
    }

    public void SetPokemon(UPokemon pokemon, int index)
    {
        Pokemon = pokemon;
        _pokemonIndex = index;
        Refresh();
    }

    [UFunction(
        FunctionFlags.BlueprintCallable | FunctionFlags.BlueprintEvent,
        Category = "Display"
    )]
    public virtual void Refresh()
    {
        // No native implementation
    }
}
