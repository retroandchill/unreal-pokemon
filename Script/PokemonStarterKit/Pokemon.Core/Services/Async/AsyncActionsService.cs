using Pokemon.Core.Entities;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Core;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Services.Async;

/// <summary>
/// Represents information about an item slot in a player's inventory or bag.
/// </summary>
/// <remarks>
/// This structure combines an item handle with its associated quantity. It is mainly utilized
/// for tracking specific item instances and their counts in systems such as inventory management
/// or user interface operations.
/// </remarks>
/// <param name="Item">
/// The handle to the item, which references its identifier or metadata. This handle provides
/// a lightweight and efficient way to represent item definitions.
/// </param>
/// <param name="Quantity">
/// The quantity of the item in the slot, which indicates how many instances of
/// the referenced item handle are present.
/// </param>
[UStruct]
public readonly partial record struct FItemSlotInfo(FItemHandle Item, int Quantity);

/// <summary>
/// Defines a service contract for handling asynchronous actions within the game framework.
/// </summary>
/// <remarks>
/// This interface provides methods for various user interaction flows, such as displaying messages,
/// managing Pokémon-related operations, and handling item usage within the player's party or bag.
/// It is designed to be implemented for integrating game-specific UI behaviors or interaction handling.
/// </remarks>
public interface IAsyncActionsService
{
    /// <summary>
    /// Displays a message to the player asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller to which the message will be displayed.</param>
    /// <param name="message">The text message to display to the player.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task representing the asynchronous operation.</returns>
    Task ShowMessageAsync(
        APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Displays a choice prompt to the player and captures their selection.
    /// </summary>
    /// <param name="playerController">The player controller to which the choice prompt will be displayed.</param>
    /// <param name="message">The message to show above the choice options.</param>
    /// <param name="choices">A read-only list of choices to present to the player.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task representing the asynchronous operation, returning the index of the player's choice.</returns>
    Task<int> ShowChoicePrompt(
        APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Displays a Yes/No prompt to the player asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller to which the Yes/No prompt will be displayed.</param>
    /// <param name="message">The message to display in the prompt.</param>
    /// <param name="yesText">The text for the "Yes" option.</param>
    /// <param name="noText">The text for the "No" option.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task that represents the asynchronous operation, containing a boolean value indicating whether the "Yes" option was chosen.</returns>
    Task<bool> ShowYesNoPromptAsync(
        APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Allows the player to select a Pokémon from their party asynchronously.
    /// </summary>
    /// <param name="playerController">The controller for the player making the selection.</param>
    /// <param name="cancellationToken">A token to monitor for cancellation requests.</param>
    /// <returns>A task that represents the asynchronous operation. The task result contains the selected Pokémon or null if no Pokémon is selected.</returns>
    Task<UPokemon?> SelectPokemonFromPartyAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Allows the player to select an item from their bag asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller managing the player selecting the item.</param>
    /// <param name="pocketFilter">Optional filter to restrict items to a specific bag pocket.</param>
    /// <param name="filter">Optional predicate to further filter eligible items for selection.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task that represents the asynchronous operation. The task result contains information about the selected item,
    /// or null if no item was selected.</returns>
    Task<FItemSlotInfo?> SelectItemFromBagAsync(
        APlayerController playerController,
        FGameplayTag? pocketFilter = null,
        Func<FItemHandle, bool>? filter = null,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Prompts the player to decide whether to replace an existing move for a Pokémon.
    /// </summary>
    /// <param name="playerController">The player controller associated with the player making the decision.</param>
    /// <param name="pokemon">The Pokémon for which the move replacement is being considered.</param>
    /// <param name="move">The move that is being proposed for replacement.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task that represents the asynchronous operation, containing a boolean result indicating whether the move should be replaced.</returns>
    Task<bool> PromptReplaceMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Opens the player's bag and allows an item to be given to the specified Pokémon asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller handling the interaction.</param>
    /// <param name="pokemon">The Pokémon to which the item will be given.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task that returns true if an item was successfully given to the Pokémon; otherwise, false.</returns>
    Task<bool> GivePokemonItemFromBagAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Gives an item to the specified Pokemon asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller initiating the action.</param>
    /// <param name="pokemon">The Pokemon to which the item will be given.</param>
    /// <param name="item">The item to give to the Pokemon.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task representing the asynchronous operation, containing a boolean value indicating whether the item was successfully given to the Pokemon.</returns>
    Task<bool> GiveItemToPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Asynchronously removes an item held by the specified Pokemon.
    /// </summary>
    /// <param name="playerController">The player controller initiating the action.</param>
    /// <param name="pokemon">The Pokemon from which the item will be removed.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task representing the asynchronous operation, containing a boolean indicating whether the item was successfully removed.</returns>
    Task<bool> TakeItemFromPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    );

    /// <summary>
    /// Allows a Pokémon to learn a new move asynchronously.
    /// </summary>
    /// <param name="playerController">The player controller operating the action.</param>
    /// <param name="pokemon">The Pokémon that will learn the move.</param>
    /// <param name="move">The move to be learned by the Pokémon.</param>
    /// <param name="cancellationToken">The token to monitor for cancellation requests.</param>
    /// <returns>A task that returns a boolean indicating whether the Pokémon successfully learned the move.</returns>
    Task<bool> LearnMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    );
}
