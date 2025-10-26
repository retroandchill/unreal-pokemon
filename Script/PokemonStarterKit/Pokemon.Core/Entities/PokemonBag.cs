using JetBrains.Annotations;
using Pokemon.Core.Components.Bag;
using Pokemon.Data.Model.PBS;
using RPG.SourceGenerator.Attributes;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.CoreUObject;
using UnrealSharp.GameplayTags;
using UnrealSharp.RPGCore;

namespace Pokemon.Core.Entities;

/// <summary>
/// Represents a unique identifier for a pocket within a Pokemon Bag.
/// </summary>
/// <remarks>
/// Uses a gameplay tag and a localized name to uniquely identify a pocket.
/// This is used to organize and manage items stored in specific sections of the bag.
/// </remarks>
[UStruct]
public readonly partial record struct FPocketIdentifier(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FGameplayTag Tag,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FText Name
);

/// <summary>
/// Represents an individual slot for holding an item and its quantity in a Pokemon Bag.
/// </summary>
/// <remarks>
/// Combines an item handle with a specified quantity to define a single entry in a pocket or container.
/// Used to organize and manage item instances within the bag system.
/// </remarks>
[UStruct]
public readonly partial record struct FItemSlot(
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] FItemHandle Item,
    [property: UProperty(PropertyFlags.BlueprintReadOnly)] int Quantity
);

/// <summary>
/// Represents initialization parameters required for configuring a Pokémon Bag.
/// </summary>
/// <remarks>
/// This struct provides the necessary data for setting up a bag, including the definition of pockets and the maximum number of items per slot.
/// It ensures a proper structure for managing items in the bag system.
/// Typically passed during bag creation or initialization processes.
/// </remarks>
[UStruct]
public readonly partial record struct FBagInitParams(IReadOnlyList<FPocketSetting> Pockets, int MaxItemsPerSlot);

/// <summary>
/// Represents a Pokemon bag entity used for managing and organizing items in a Pokemon game.
/// </summary>
/// <remarks>
/// UPokemonBag serves as an advanced entity for handling item-related functionalities such as
/// managing pockets, item memory, and item registrations. It extends the URPGEntity base class
/// and provides integrations with specific components defined in the game, including pockets,
/// memory, and registration management. This class also supports the creation of Pokemon bags
/// with customizable initialization parameters.
/// </remarks>
[UClass(ClassFlags.Abstract)]
public partial class UPokemonBag : URPGEntity
{
    /// <summary>
    /// Represents a component of a Pokémon bag responsible for managing and organizing pockets.
    /// </summary>
    /// <remarks>
    /// This component provides functionality to initialize pockets within a Pokémon bag using specified parameters.
    /// It is primarily used as part of the Pokémon bag framework within the RPG system.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UPocketsComponent.Initialize))]
    public partial UPocketsComponent PocketsComponent { get; private set; }

    /// <summary>
    /// Represents a component of a Pokémon bag responsible for managing memory related to pockets.
    /// </summary>
    /// <remarks>
    /// This component is utilized to handle the initialization and organization of pocket-related data within a Pokémon bag.
    /// It facilitates functionality critical for maintaining the state and configuration of pockets in the RPG framework.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    [Initializer(nameof(UPocketMemoryComponent.Initialize))]
    public partial UPocketMemoryComponent PocketMemoryComponent { get; private set; }

    /// <summary>
    /// Represents a component responsible for handling the registration and management of items within the Pokémon bag system.
    /// </summary>
    /// <remarks>
    /// This component facilitates the functionality required to register, organize, and manage items, ensuring integration within the Pokémon RPG framework.
    /// It is designed to operate as part of the overall item management system within the bag's architecture.
    /// </remarks>
    [UProperty(
        PropertyFlags.EditDefaultsOnly | PropertyFlags.BlueprintReadOnly | PropertyFlags.Instanced,
        Category = "Components"
    )]
    public partial UItemRegistrationComponent ItemRegistrationComponent { get; private set; }

    /// <summary>
    /// Creates a new instance of <see cref="UPokemonBag"/> for the specified outer object, using default bag initialization parameters.
    /// </summary>
    /// <param name="outer">The outer object for this <see cref="UPokemonBag"/> instance, typically the owning subsystem or game object.</param>
    /// <returns>A new <see cref="UPokemonBag"/> instance configured with default initialization parameters.</returns>
    public static UPokemonBag Create(UObject outer)
    {
        var settings = GetDefault<UPokemonCoreSettings>();
        return Create(outer, new FBagInitParams(settings.Pockets, settings.MaxPerSlot));
    }

    /// <summary>
    /// Creates a new instance of <see cref="UPokemonBag"/> with the specified outer object and initialization parameters.
    /// </summary>
    /// <param name="outer">The outer object for this <see cref="UPokemonBag"/> instance, typically representing the owning subsystem or game object.</param>
    /// <param name="parameters">The initialization parameters for configuring this <see cref="UPokemonBag"/>, including pocket settings and maximum items per slot.</param>
    /// <returns>A new <see cref="UPokemonBag"/> instance configured with the provided parameters.</returns>
    [Factory]
    [PublicAPI]
    public static partial UPokemonBag Create(UObject outer, FBagInitParams parameters);

    private static TSubclassOf<UPokemonBag> BagClass => GetDefault<UPokemonCoreSettings>().BagClass.LoadSynchronous();
}
