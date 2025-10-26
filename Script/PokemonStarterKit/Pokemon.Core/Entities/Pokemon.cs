using JetBrains.Annotations;
using Pokemon.Core.Components.Pokemon;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

/// <summary>
/// Represents the initialization parameters for creating a new Pokemon entity.
/// </summary>
/// <remarks>
/// This structure is used to encapsulate the necessary data required to initialize a Pokemon,
/// including its species and level.
/// </remarks>
[UStruct]
public readonly partial record struct FPokemonInitParams(
    [property: UProperty] FSpeciesHandle Species,
    [property: UProperty] int Level
);

/// <summary>
/// Represents a Pokemon entity within the game world.
/// </summary>
/// <remarks>
/// This class is responsible for managing the core components and functionality of a Pokemon, such as its identity, ownership, stats, abilities, moves, held items, and status effects.
/// It also defines methods for combat-related logic and interactions, such as healing and determining if the Pokemon is able to battle.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public partial class UPokemon : URPGEntity
{
    /// <summary>
    /// Represents the component used to define and manage the identity-related attributes
    /// and functionality for a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This component is part of the Pokémon core entity architecture and provides key
    /// identity-specific operations, such as initialization through defined parameters.
    /// It is marked as editable only within default configurations and can be used in
    /// both Blueprint and runtime contexts.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UIdentityComponent.Initialize))]
    public partial UIdentityComponent IdentityComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for managing ownership attributes and functionality
    /// for a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This component is a core part of the Pokémon entity framework, intended to handle
    /// ownership details, including initialization through specific parameters. It is marked
    /// as editable in default configurations and accessible in both Blueprint and runtime contexts.
    /// Metadata restrictions have been applied to hide certain categories related to ownership
    /// and trainer details for refinement in its usage.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UOwnershipComponent.Initialize))]
    public partial UOwnershipComponent OwnershipComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for managing a Pokémon entity's statistical attributes
    /// and functionality, such as health, attack, defense, and other related metrics.
    /// </summary>
    /// <remarks>
    /// This component plays a vital role in the Pokémon core architecture by enabling functionality
    /// related to battle mechanics, healing processes, and other stat-driven operations.
    /// It is instantiated and initialized with specific parameters during the entity setup phase.
    /// Configured for use in both Blueprint and runtime environments with restricted editing options
    /// to maintain consistency.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UStatComponent.Initialize))]
    public partial UStatComponent StatComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for handling and managing the abilities
    /// associated with a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This component is integral to the Pokémon's functionality, providing mechanisms
    /// for initializing and managing abilities. It is designed to be edited in default
    /// configurations only and can be utilized in both Blueprint and runtime contexts.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UAbilityComponent.Initialize))]
    public partial UAbilityComponent AbilityComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for managing and executing
    /// the move-related functionality for a Pokémon entity.
    /// </summary>
    /// <remarks>
    /// This component serves as an integral part of a Pokémon's behavior,
    /// enabling the initialization, definition, and handling of moves within the system.
    /// It is designed for use in both default configuration settings and runtime operations,
    /// adhering to the broader Pokémon entity architecture.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UMoveComponent.Initialize))]
    public partial UMoveComponent MoveComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for managing and defining the behavior
    /// and attributes of held items for a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This component is integrated into the Pokémon entity framework and allows for the
    /// customization and interaction of holdable items. It supports configurations through
    /// specified default properties and can be utilized in both Blueprint and runtime contexts.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UHoldItemComponent HoldItemComponent { get; private set; }

    /// <summary>
    /// Represents the component responsible for managing status effects
    /// and related functionality for a Pokémon entity within the system.
    /// </summary>
    /// <remarks>
    /// This component is part of the Pokémon core entity architecture and
    /// provides mechanisms to handle the application, removal, and tracking
    /// of various status effects. It is designed to integrate with healing
    /// systems and can be utilized in both Blueprint and runtime contexts.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UStatusEffectComponent StatusEffectComponent { get; private set; }

    /// <summary>
    /// Indicates whether the Pokémon is currently able to participate in a battle.
    /// </summary>
    /// <remarks>
    /// This property evaluates all relevant battle-related components of the Pokémon to determine
    /// its readiness or eligibility for battle. If all components that define battle capability report
    /// readiness, the Pokémon is considered able to battle.
    /// </remarks>
    public bool IsAbleToBattle
    {
        [UFunction(FunctionFlags.BlueprintPure, DisplayName = "Is Able to Battle", Category = "Battle")]
        get { return AllComponents.OfType<IBattleCapableComponent>().All(c => c.IsAbleToBattle); }
    }

    /// <summary>
    /// Creates a new instance of the UPokemon class with the specified trainer, species, and level.
    /// </summary>
    /// <param name="outer">The trainer that owns the Pokémon instance.</param>
    /// <param name="species">The species of the Pokémon to create.</param>
    /// <param name="level">The level of the Pokémon being created.</param>
    /// <returns>Returns a new instance of the UPokemon class initialized with the specified parameters.</returns>
    [PublicAPI]
    public static UPokemon Create(UTrainer outer, FSpeciesHandle species, int level)
    {
        return Create(outer, new FPokemonInitParams(species, level));
    }

    /// <summary>
    /// Creates a new instance of the UPokemon class with the specified trainer and initialization parameters.
    /// </summary>
    /// <param name="outer">The trainer that will own the Pokémon instance.</param>
    /// <param name="parameters">The initialization parameters, including species and level, for the Pokémon instance.</param>
    /// <returns>Returns a new instance of the UPokemon class initialized with the specified trainer and parameters.</returns>
    [Factory]
    [PublicAPI]
    public static partial UPokemon Create(UTrainer outer, FPokemonInitParams parameters);

    private static TSubclassOf<UPokemon> PokemonClass =>
        GetDefault<UPokemonCoreSettings>().PokemonClass.LoadSynchronous();

    /// <summary>
    /// Restores the health of all healable components associated with the Pokémon.
    /// </summary>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Stats")]
    public void Heal()
    {
        foreach (var component in AllComponents.OfType<IHealableComponent>())
        {
            component.Heal();
        }
    }
}
