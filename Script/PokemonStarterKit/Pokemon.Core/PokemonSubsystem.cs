using Microsoft.Extensions.DependencyInjection;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Core.Services.Exp;
using Pokemon.Core.Services.Stats;
using Pokemon.Data.Model.HardCoded;
using UnrealInject.Subsystems;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.CoreUObject;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;
using UnrealSharp.UnrealSharpCore;

namespace Pokemon.Core;

/// <summary>
/// Represents the player's reset location within the game.
/// </summary>
/// <remarks>
/// This structure stores the map name and the player's transform for resetting their location.
/// It can be used in scenarios such as respawning or reinitializing the player position in the game world.
/// </remarks>
/// <param name="MapName">The name of the map where the player's reset location is stored.</param>
/// <param name="PlayerTransform">
/// The transform data (including location, rotation, and scale) associated with the player's reset position.
/// </param>
public readonly record struct FPlayerResetLocation(string MapName, FTransform PlayerTransform);

/// <summary>
/// A subsystem that manages core Pokémon mechanics within the game instance.
/// </summary>
/// <remarks>
/// This class is responsible for handling various game systems related to the Pokémon gameplay, such as the player's trainer information, Pokémon bag, experience growth formulas, and location management.
/// It integrates with other services to ensure smooth functionality, including asynchronous actions and stat calculations.
/// </remarks>
[UClass(DisplayName = "Pokémon Subsystem")]
public class UPokemonSubsystem : UCSGameInstanceSubsystem
{
    private readonly Dictionary<FGrowthRateHandle, IExpGrowthFormula> _expGrowthFormulas = new();

    /// <summary>
    /// Represents the player in the Pokémon subsystem.
    /// This property provides access to the current trainer instance used in the game.
    /// It is a BlueprintReadOnly property, meaning it can only be read within the Blueprint environment but modified internally.
    /// The Player is initialized during the start of a new game, and its instance is associated with the current game session.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UTrainer Player { get; private set; }

    /// <summary>
    /// Represents the bag holding items for the player in the Pokémon subsystem.
    /// This property provides access to the player's item storage, allowing retrieval and management of in-game items.
    /// It is a BlueprintReadOnly property, meaning it can only be read within the Blueprint environment but modified internally.
    /// The Bag is initialized at the start of a new game session and is tied to the current instance of the Pokémon subsystem.
    /// </summary>
    [UProperty(PropertyFlags.BlueprintReadOnly, Category = "Player")]
    public UPokemonBag Bag { get; private set; }

    /// <summary>
    /// Provides access to the service responsible for calculating Pokémon stats dynamically within the game.
    /// This service handles computations for attributes such as base stats, level scaling, IVs, EVs, and nature modifiers.
    /// It is initialized as part of the game instance subsystem and ensures stat adjustments are applied consistently across the gameplay.
    /// </summary>
    public IStatCalculationService StatCalculationService { get; private set; }

    private FTransform? _playerLoadTransform;

    private FPlayerResetLocation? _playerResetLocation;

    /// <summary>
    /// Indicates whether the player's reset location has been configured in the game.
    /// This property checks if the reset location, which includes the player's map and transform, has been initialized.
    /// It is primarily used to determine if the player's respawn or fallback point is set within the subsystem.
    /// </summary>
    public bool IsPlayerResetLocationSet
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Player Reset Location Set", Category = "Location")]
        get => _playerResetLocation.HasValue;
    }

    /// <summary>
    /// Represents the current location in the Pokémon subsystem.
    /// This property provides the name or identifier of the player's present in-game location.
    /// It is exposed as a BlueprintReadOnly property, allowing it to be accessed in the Blueprint environment but not modified externally.
    /// The CurrentLocation is dynamically updated based on the player's movements and map transitions throughout the game.
    /// </summary>
    public FText CurrentLocation
    {
        // TODO: Implement this
        [UFunction(FunctionFlags.BlueprintPure, Category = "Location")]
        get => "Test Location";
    }

    /// <summary>
    /// Provides access to the asynchronous actions service within the Pokémon subsystem.
    /// This property is used to manage and execute asynchronous tasks or operations
    /// throughout the subsystem. It is initialized as part of the subsystem's setup
    /// process and is essential for handling non-blocking tasks effectively.
    /// </summary>
    public IAsyncActionsService AsyncActionsService { get; private set; } = null!;

    /// <inheritdoc />
    protected override void Initialize(FSubsystemCollectionBaseRef collection)
    {
        var subsystem = collection.InitializeRequiredSubsystem<UDependencyInjectionGameInstanceSubsystem>();
        foreach (var expGrowthFormula in subsystem.GetServices<IExpGrowthFormula>())
        {
            _expGrowthFormulas.Add(expGrowthFormula.GrowthRateFor, expGrowthFormula);
        }
        AsyncActionsService = subsystem.GetRequiredService<IAsyncActionsService>();
        StatCalculationService = subsystem.GetRequiredService<IStatCalculationService>();

        // TODO: Remove this and create a proper title screen
        StartNewGame();
    }

    /// <inheritdoc />
    protected override void Deinitialize()
    {
        _expGrowthFormulas.Clear();
    }

    /// <summary>
    /// Retrieves the experience growth formula associated with the provided growth rate handle.
    /// </summary>
    /// <param name="growthRate">The handle representing the growth rate for which the experience growth formula is requested.</param>
    /// <returns>The experience growth formula that corresponds to the specified growth rate handle.</returns>
    /// <exception cref="InvalidOperationException">Thrown if no formula is found for the given growth rate handle.</exception>
    public IExpGrowthFormula GetExpGrowthFormula(FGrowthRateHandle growthRate)
    {
        return _expGrowthFormulas.TryGetValue(growthRate, out var formula)
            ? formula
            : throw new InvalidOperationException($"No formula for growth rate {growthRate}");
    }

    /// <summary>
    /// Initializes a new game by creating the player and their Pokémon bag.
    /// This method sets up the initial state of the game by instantiating the trainer
    /// and populating the player's inventory.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Playthrough")]
    public void StartNewGame()
    {
        // TODO: Swap this instantiation with the actual trainer instantiation
        Player = UTrainer.Create(this, new FName("POKEMONTRAINER_Nate"), "Nate");
        Bag = UPokemonBag.Create(this);
    }

    /// <summary>
    /// Adjusts the player's transform to a predefined transform when the game is loaded.
    /// </summary>
    /// <param name="playerCharacter">The player character whose transform needs to be adjusted.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void AdjustPlayerTransformOnLoad(ACharacter playerCharacter)
    {
        if (!_playerLoadTransform.HasValue)
            return;

        playerCharacter.SetActorTransform(_playerLoadTransform.Value);
        _playerLoadTransform = null;
    }

    /// <summary>
    /// Sets the player's reset location to a specified map and transform.
    /// </summary>
    /// <param name="mapName">The name of the map for the reset location.</param>
    /// <param name="transform">The transform (position, rotation, and scale) to be used for the reset location.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void SetPlayerResetLocation(string mapName, FTransform transform)
    {
        _playerResetLocation = new FPlayerResetLocation(mapName, transform);
    }

    /// <summary>
    /// Sets the player's reset location to their current location.
    /// </summary>
    /// <param name="playerCharacter">The player character whose current location will be set as the reset location.</param>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Player")]
    public void SetPlayerResetLocationAsCurrentLocation(ACharacter playerCharacter)
    {
        // TODO: Implement this
    }
}
