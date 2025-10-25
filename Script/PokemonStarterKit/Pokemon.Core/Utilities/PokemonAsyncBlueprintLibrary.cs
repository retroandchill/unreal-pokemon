using LanguageExt;
using Pokemon.Core.Entities;
using Pokemon.Core.Services.Async;
using Pokemon.Data.Model.PBS;
using UnrealSharp;
using UnrealSharp.Attributes;
using UnrealSharp.Attributes.MetaTags;
using UnrealSharp.Engine;
using UnrealSharp.GameplayTags;

namespace Pokemon.Core.Utilities;

/// <summary>
/// Represents the result of a selection operation.
/// </summary>
[UEnum]
public enum ESelectionResult : byte
{
    /// <summary>
    /// An option was selected
    /// </summary>
    Selected,

    /// <summary>
    /// The selection was cancelled.
    /// </summary>
    Cancelled,
}

/// <summary>
/// Represents a selected Pokémon entity.
/// </summary>
/// <remarks>
/// This struct is used to encapsulate the details of a Pokémon selected by the user during gameplay.
/// It primarily contains a reference to the selected Pokémon, or it may be null if no Pokémon is selected.
/// </remarks>
[UStruct]
public readonly partial record struct FSelectedPokemon([UProperty] UPokemon? Pokemon);

/// <summary>
/// Represents a selected item entity.
/// </summary>
/// <remarks>
/// This struct is utilized to encapsulate the details of an item selected by the user from their bag or inventory.
/// It contains an optional reference to the selected item's slot information, such as the item itself and its quantity.
/// </remarks>
[UStruct]
public readonly partial record struct FSelectedItem([UProperty] Option<FItemSlotInfo> Item);

/// <summary>
/// Represents a filter criteria that determines whether a specific item meets the desired conditions.
/// </summary>
/// <param name="item">The item to be filtered.</param>
[USingleDelegate]
public delegate bool ItemFilter(FItemHandle item);

/// <summary>
/// Provides a set of asynchronous utility functions for interacting with Pokémon gameplay elements in Unreal Engine Blueprint scripting.
/// </summary>
/// <remarks>
/// The <c>UPokemonAsyncBlueprintLibrary</c> class offers a collection of static methods designed to handle various gameplay operations asynchronously,
/// such as displaying prompts, managing Pokémon items, or learning moves.
/// These methods are intended to simplify interactions within gameplay systems while leveraging asynchronous patterns.
/// This library is designed for use in Unreal Engine projects with Blueprint functionality.
/// </remarks>
[UClass]
public class UPokemonAsyncBlueprintLibrary : UBlueprintFunctionLibrary
{
    /// <summary>
    /// Displays a message asynchronously to the player controlled by the specified player controller.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player to whom the message should be displayed.
    /// </param>
    /// <param name="message">
    /// The message to display to the player.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the message display operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of displaying the message.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task ShowMessageAsync(
        APlayerController playerController,
        FText message,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowMessageAsync(message, cancellationToken);
    }

    /// <summary>
    /// Displays a choice prompt asynchronously to the player controlled by the specified player controller.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player to whom the choice prompt will be displayed.
    /// </param>
    /// <param name="message">
    /// The message to display as part of the choice prompt.
    /// </param>
    /// <param name="choices">
    /// A list of choices from which the player can select one.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the choice prompt operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of displaying the choice prompt, which on completion returns the zero-based index of the choice selected by the player.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<int> ShowChoicePrompt(
        APlayerController playerController,
        FText message,
        IReadOnlyList<FText> choices,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowChoicePrompt(message, choices, cancellationToken);
    }

    /// <summary>
    /// Displays a Yes/No prompt asynchronously to the player controlled by the specified player controller.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player to whom the prompt should be displayed.
    /// </param>
    /// <param name="message">
    /// The message to display within the Yes/No prompt.
    /// </param>
    /// <param name="yesText">
    /// The text to display for the "Yes" option in the prompt.
    /// </param>
    /// <param name="noText">
    /// The text to display for the "No" option in the prompt.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the prompt display operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of displaying the Yes/No prompt.
    /// The task result is a boolean value indicating the player's choice. True if "Yes" was selected, and False if "No" was selected.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> ShowYesNoPromptAsync(
        APlayerController playerController,
        FText message,
        FText yesText,
        FText noText,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.ShowYesNoPromptAsync(message, yesText, noText, cancellationToken);
    }

    /// <summary>
    /// Asynchronously allows the player to select a Pokémon from their party.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player's input for selecting a Pokémon.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the Pokémon selection operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of Pokémon selection, returning the selected Pokémon as part of the result.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<FSelectedPokemon> SelectPokemonFromPartyAsync(
        APlayerController playerController,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.SelectPokemonFromPartyAsync(cancellationToken).Map(p => new FSelectedPokemon(p));
    }

    /// <summary>
    /// Matches the selected Pokémon and determines if a Pokémon was successfully selected.
    /// </summary>
    /// <param name="selectedPokemon">
    /// The structure containing the selected Pokémon.
    /// </param>
    /// <param name="pokemon">
    /// The out parameter that will hold the matched Pokémon if a valid selection was made; otherwise, null.
    /// </param>
    /// <returns>
    /// An <see cref="ESelectionResult"/> indicating the result of the matching operation. It returns <c>Selected</c> if a Pokémon was successfully matched, otherwise <c>Cancelled</c>.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, DisplayName = "Match (Selected Pokémon)", Category = "Async")]
    [ExpandEnumAsExecs("ReturnValue")]
    public static ESelectionResult MatchSelectedPokemon(FSelectedPokemon selectedPokemon, out UPokemon? pokemon)
    {
        pokemon = selectedPokemon.Pokemon;
        return pokemon is not null ? ESelectionResult.Selected : ESelectionResult.Cancelled;
    }

    /// <summary>
    /// Allows the player to select an item from their bag asynchronously, using optional filters to constrain
    /// selectable items.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player selecting the item.
    /// </param>
    /// <param name="pocketFilter">
    /// A gameplay tag specifying the pocket or category of items to filter by (e.g., Items, TM/HMs).
    /// </param>
    /// <param name="filter">
    /// A custom delegate filter to further restrict selectable items based on custom logic.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the item selection process.
    /// </param>
    /// <returns>
    /// A task representing the asynchronous operation that returns a selected item wrapped in a
    /// <c>FSelectedItem</c> structure. If no item is selected, the result will encapsulate an empty option.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, DisplayName = "Match (Selected Pokémon)", Category = "Async")]
    public static Task<FSelectedItem> SelectItemFromBagAsync(
        APlayerController playerController,
        FGameplayTag pocketFilter,
        TDelegate<ItemFilter> filter,
        CancellationToken cancellationToken = default
    )
    {
        return playerController
            .SelectItemFromBagAsync(
                pocketFilter.IsValid ? pocketFilter : null,
                i => !filter.IsBound || filter.InnerDelegate.Invoke(i),
                cancellationToken
            )
            .Map(i => new FSelectedItem(i.ToOption()));
    }

    /// <summary>
    /// Matches the selected item and extracts its details, including the item handle and quantity.
    /// </summary>
    /// <param name="selectedItem">
    /// The selected item structure containing information about the chosen item.
    /// </param>
    /// <param name="item">
    /// The handle of the selected item. If no item is selected, this will be set to its default value.
    /// </param>
    /// <param name="quantity">
    /// The quantity of the selected item. If no item is selected, this will be set to 0.
    /// </param>
    /// <returns>
    /// An enumeration value indicating the result of the selection process. Returns <c>ESelectionResult.Selected</c> if an item is successfully matched, otherwise returns <c>ESelectionResult.Cancelled</c>.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, DisplayName = "Match (Selected Item)", Category = "Async")]
    [ExpandEnumAsExecs("ReturnValue")]
    public static ESelectionResult MatchSelectedItem(FSelectedItem selectedItem, out FItemHandle item, out int quantity)
    {
        var slot = selectedItem.Item.ToNullable();
        if (slot.HasValue)
        {
            item = slot.Value.Item;
            quantity = slot.Value.Quantity;
            return ESelectionResult.Selected;
        }

        item = default;
        quantity = 0;
        return ESelectionResult.Cancelled;
    }

    /// <summary>
    /// Prompts the player to replace a move for the given Pokémon asynchronously.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player being prompted to replace the move.
    /// </param>
    /// <param name="pokemon">
    /// The Pokémon for which the move is being replaced.
    /// </param>
    /// <param name="move">
    /// The handle of the move to be replaced.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the prompt operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation. The task result indicates whether the player agreed to replace the move.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> PromptReplaceMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.PromptReplaceMoveAsync(pokemon, move, cancellationToken);
    }

    /// <summary>
    /// Gives an item to the specified Pokémon from the player's bag asynchronously.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player attempting to give the item.
    /// </param>
    /// <param name="pokemon">
    /// The Pokémon that will receive the item.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the operation.
    /// </param>
    /// <returns>
    /// A task representing the asynchronous operation, containing a boolean value indicating whether the item was successfully given to the Pokémon.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> GivePokemonItemFromBagAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.GivePokemonItemFromBagAsync(pokemon, cancellationToken);
    }

    /// <summary>
    /// Asynchronously gives an item to a specified Pokémon managed by the given player controller.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the Pokémon to which the item should be given.
    /// </param>
    /// <param name="pokemon">
    /// The Pokémon to receive the item.
    /// </param>
    /// <param name="item">
    /// The item handle representing the item to be given.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the item-giving operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of giving the item to the Pokémon. Returns a boolean indicating whether the item was successfully given.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> GiveItemToPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FItemHandle item,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.GiveItemToPokemonAsync(pokemon, item, cancellationToken);
    }

    /// <summary>
    /// Attempts to take an item from the specified Pokémon asynchronously.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the player initiating the item retrieval operation.
    /// </param>
    /// <param name="pokemon">
    /// The Pokémon from which the item will be taken.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the item retrieval operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation of taking the item from the Pokémon.
    /// The task result is <c>true</c> if the item was successfully taken; otherwise, <c>false</c>.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> TakeItemFromPokemonAsync(
        APlayerController playerController,
        UPokemon pokemon,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.TakeItemFromPokemonAsync(pokemon, cancellationToken);
    }

    /// <summary>
    /// Allows a Pokémon to learn a specific move asynchronously.
    /// </summary>
    /// <param name="playerController">
    /// The player controller managing the operation for teaching the move to the Pokémon.
    /// </param>
    /// <param name="pokemon">
    /// The Pokémon that is intended to learn the specified move.
    /// </param>
    /// <param name="move">
    /// The move that the Pokémon should learn.
    /// </param>
    /// <param name="cancellationToken">
    /// An optional token to observe for cancellation of the move learning operation.
    /// </param>
    /// <returns>
    /// A task that represents the asynchronous operation to teach the move. The result contains a boolean indicating whether the move was learned successfully.
    /// </returns>
    [UFunction(FunctionFlags.BlueprintCallable, Category = "Async")]
    public static Task<bool> LearnMoveAsync(
        APlayerController playerController,
        UPokemon pokemon,
        FMoveHandle move,
        CancellationToken cancellationToken = default
    )
    {
        return playerController.LearnMoveAsync(pokemon, move, cancellationToken);
    }
}
