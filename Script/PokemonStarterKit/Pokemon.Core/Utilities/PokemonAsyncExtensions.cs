using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Utilities;

/// <summary>
/// Provides a set of asynchronous extension methods for interacting with Pokémon-related functionalities
/// in the context of a player controller.
/// </summary>
public static class PokemonAsyncExtensions
{
    private static IAsyncActionsService GetActionService(APlayerController playerController)
    {
        return playerController.GetGameInstanceSubsystem<UPokemonSubsystem>().AsyncActionsService;
    }

    /// <summary>
    /// Asynchronously displays a message to the player controller.
    /// </summary>
    /// <param name="playerController">The player controller for which the message is displayed.</param>
    /// <param name="message">The message to display.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation.</returns>
    public static Task ShowMessageAsync(
        this APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowMessageAsync(playerController, message, cancellationToken);
    }

    /// <summary>
    /// Asynchronously displays a choice prompt to the player controller.
    /// </summary>
    /// <param name="playerController">The player controller for which the choice prompt is displayed.</param>
    /// <param name="message">The message to display in the choice prompt.</param>
    /// <param name="choices">The list of choices available to the player.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation, containing the index of the selected choice.</returns>
    public static Task<int> ShowChoicePrompt(
        this APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowChoicePrompt(playerController, message, choices, cancellationToken);
    }

    /// <summary>
    /// Asynchronously displays a yes/no prompt to the player controller.
    /// </summary>
    /// <param name="playerController">The player controller for which the prompt is displayed.</param>
    /// <param name="message">The message to display in the prompt.</param>
    /// <param name="yesText">The text to display for the "Yes" option.</param>
    /// <param name="noText">The text to display for the "No" option.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation, with a boolean result indicating whether the "Yes" option was selected.</returns>
    public static Task<bool> ShowYesNoPromptAsync(
        this APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.ShowYesNoPromptAsync(playerController, message, yesText, noText, cancellationToken);
    }

    /// <summary>
    /// Asynchronously allows the player to select a Pokémon from their party.
    /// </summary>
    /// <param name="playerController">The player controller making the selection.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task that represents the asynchronous operation. The task result is the selected Pokémon, or null if no selection is made.</returns>
    public static Task<UPokemon?> SelectPokemonFromPartyAsync(
        this APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.SelectPokemonFromPartyAsync(playerController, cancellationToken);
    }

    /// <summary>
    /// Asynchronously allows the player controller to select an item from the bag.
    /// </summary>
    /// <param name="playerController">The player controller initiating the item selection.</param>
    /// <param name="pocketFilter">An optional filter specifying the item pocket to search within.</param>
    /// <param name="filter">An optional predicate function used to further filter selectable items.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation, with a nullable result containing the selected item slot information or null if no item was selected.</returns>
    public static Task<FItemSlotInfo?> SelectItemFromBagAsync(
        this APlayerController playerController,
        FGameplayTag? pocketFilter = null,
        Func<FItemHandle, bool>? filter = null,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.SelectItemFromBagAsync(playerController, pocketFilter, filter, cancellationToken);
    }

    /// <summary>
    /// Asynchronously prompts the player to replace a move for a specified Pokémon.
    /// </summary>
    /// <param name="playerController">The player controller to present the prompt to.</param>
    /// <param name="pokemon">The Pokémon for which the move replacement is being prompted.</param>
    /// <param name="move">The move to potentially replace.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task representing the asynchronous operation, containing a boolean indicating whether the move was replaced.</returns>
    public static Task<bool> PromptReplaceMoveAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.PromptReplaceMoveAsync(playerController, pokemon, move, cancellationToken);
    }

    /// <summary>
    /// Asynchronously gives an item from the player's bag to the specified Pokémon.
    /// </summary>
    /// <param name="playerController">The player controller executing the action.</param>
    /// <param name="pokemon">The Pokémon to receive the item.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task that represents the asynchronous operation. The task result is a boolean indicating whether the operation was successful.</returns>
    public static Task<bool> GivePokemonItemFromBagAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.GivePokemonItemFromBagAsync(playerController, pokemon, cancellationToken);
    }

    /// <summary>
    /// Asynchronously attempts to give an item to a specified Pokémon.
    /// </summary>
    /// <param name="playerController">The player controller performing the action.</param>
    /// <param name="pokemon">The Pokémon to which the item will be given.</param>
    /// <param name="item">The item to be given to the Pokémon.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task that represents the asynchronous operation, returning a boolean indicating whether the item was successfully given.</returns>
    public static Task<bool> GiveItemToPokemonAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.GiveItemToPokemonAsync(playerController, pokemon, item, cancellationToken);
    }

    /// <summary>
    /// Asynchronously removes an item held by the specified Pokémon.
    /// </summary>
    /// <param name="playerController">The player controller performing the action.</param>
    /// <param name="pokemon">The Pokémon from which the item will be taken.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task that represents the asynchronous operation. The task result is a boolean indicating whether the item was successfully taken.</returns>
    public static Task<bool> TakeItemFromPokemonAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.TakeItemFromPokemonAsync(playerController, pokemon, cancellationToken);
    }

    /// <summary>
    /// Asynchronously allows a Pokémon to learn a specified move.
    /// </summary>
    /// <param name="playerController">The player controller initiating the operation.</param>
    /// <param name="pokemon">The Pokémon that will learn the move.</param>
    /// <param name="move">The move to be learned by the Pokémon.</param>
    /// <param name="cancellationToken">An optional cancellation token to observe while waiting for the operation to complete.</param>
    /// <returns>A task that resolves to a boolean indicating whether the Pokémon successfully learned the move.</returns>
    public static Task<bool> LearnMoveAsync(
        this APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        var service = GetActionService(playerController);
        return service.LearnMoveAsync(playerController, pokemon, move, cancellationToken);
    }
}
