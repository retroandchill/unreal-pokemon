using Microsoft.Extensions.DependencyInjection;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Exp;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.Subsystems;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Core;

[UClass(DisplayName = "Pokémon Subsystem")]
public class UPokemonSubsystem : UCSGameInstanceSubsystem
{
    private readonly Dictionary<FGrowthRateHandle, IExpGrowthFormula> _expGrowthFormulas = new();

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UTrainer Player { get; private set; }
    
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UPokemonBag Bag { get; private set; }

    public FText CurrentLocation
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Location")]
        get => throw new NotImplementedException();
    }

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var subsystem =
            collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        foreach (var expGrowthFormula in subsystem.GetServices<IExpGrowthFormula>())
        {
            _expGrowthFormulas.Add(expGrowthFormula.GrowthRateFor, expGrowthFormula);
        }

        // TODO: Remove this and create a proper title screen
        StartNewGame();
    }

    protected override void Deinitialize()
    {
        _expGrowthFormulas.Clear();
    }

    public IExpGrowthFormula GetExpGrowthFormula(FGrowthRateHandle growthRate)
    {
        return _expGrowthFormulas.TryGetValue(growthRate, out var formula)
            ? formula
            : throw new InvalidOperationException($"No formula for growth rate {growthRate}");
    }

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Playthrough")]
    public void StartNewGame()
    {
        // TODO: Swap this instantiation with the actual trainer instantiation
        Player = UTrainer.Create(this, new FName("POKEMONTRAINER_Nate"), "Nate");
        Bag = UPokemonBag.Create(this);
    }
}
