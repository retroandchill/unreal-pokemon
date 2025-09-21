using Microsoft.Extensions.DependencyInjection;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Core.Services.Exp;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.Subsystems;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Core;

public readonly record struct FPlayerResetLocation(string MapName, FTransform PlayerTransform);

[UClass(DisplayName = "Pokémon Subsystem")]
public class UPokemonSubsystem : UCSGameInstanceSubsystem
{
    private readonly Dictionary<FGrowthRateHandle, IExpGrowthFormula> _expGrowthFormulas = new();

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UTrainer Player { get; private set; }

    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UPokemonBag Bag { get; private set; }

    private FTransform? _playerLoadTransform;

    private FPlayerResetLocation? _playerResetLocation;
    
    public bool IsPlayerResetLocationSet
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Player Reset Location Set", Category = "Location")]
        get => _playerResetLocation.HasValue;
    }

    public FText CurrentLocation
    {
        [UFunction(FunctionFlags.BlueprintPure, Category = "Location")]
        get => throw new NotImplementedException();
    }

    public IAsyncActionsService AsyncActionsService { get; private set; } = null!;

    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var subsystem =
            collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        foreach (var expGrowthFormula in subsystem.GetServices<IExpGrowthFormula>())
        {
            _expGrowthFormulas.Add(expGrowthFormula.GrowthRateFor, expGrowthFormula);
        }
        AsyncActionsService = subsystem.GetRequiredService<IAsyncActionsService>();

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

    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void AdjustPlayerTransformOnLoad(ACharacter playerCharacter)
    {
        if (!_playerLoadTransform.HasValue) return;
        
        playerCharacter.SetActorTransform(_playerLoadTransform.Value);
        _playerLoadTransform = null;
    }
    
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void SetPlayerResetLocation(string mapName, FTransform transform) {
        _playerResetLocation = new FPlayerResetLocation(mapName, transform);
    }


    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void SetPlayerResetLocationAsCurrentLocation(ACharacter PlayerCharacter)
    {
        // TODO: Implement this
    }
}
